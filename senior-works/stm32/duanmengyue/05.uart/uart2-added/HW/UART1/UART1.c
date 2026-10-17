/*********************************************************************************************************
* 模块名称：UART1.c
* 摘    要：串口模块，包括串口模块初始化，以及中断服务函数处理，以及读写串口函数实现
* 当前版本：1.0.0
* 作    者：DMYX
* 完成日期：2026年07月08日 
* 内    容：
* 注    意：                                                                  
**********************************************************************************************************
* 取代版本：
* 作    者：
* 完成日期：
* 修改内容：
* 修改文件：
*********************************************************************************************************/

/*********************************************************************************************************
*                                              包含头文件
*********************************************************************************************************/
#include "UART1.h"
#include "Queue.h"
#include "stm32f10x_conf.h"
#include <stdio.h>
#include "uart_common.h"
/*********************************************************************************************************
*                                              宏定义
*********************************************************************************************************/

/*********************************************************************************************************
*                                              枚举结构体定义
*********************************************************************************************************/

/*********************************************************************************************************
*                                              内部变量
*********************************************************************************************************/
static StructCirQue s_structUARTSendCirQue;  //串口发送缓冲区
static StructCirQue s_structUARTRecCirQue;   //串口接收缓冲区
static u8 s_arrSendBuf[UART1_BUF_SIZE];     //发送缓冲区数组
static u8 s_arrRecdBuf[UART1_BUF_SIZE];     //接收缓冲区数组

static u8 s_iUARTTxSts;                     //串口发送状态位，1表示正在发送数据，0表示发送数据完成
/*********************************************************************************************************
*                                              内部函数声明
*********************************************************************************************************/
static void InitUARTBuf(void);          //初始化串口缓冲区，包括发送缓冲区和接收缓冲区
static u8 WriteReceiveBuf(u8 d);        //将接收到的数据写入接收缓冲区
static u8 ReadSendBuf(u8 *p);           //读取发送缓冲区中的数据

static void ConfigUART(u32 bound);      //配置串口相关参数
static void EnableUARTTx(void);         //使能串口发送，在WriteUARTx中调用，每次发送数据之后需要调用
static void SendCharUsedByFputc(u16 ch);//发送字符函数，专由fputc函数调用
/*********************************************************************************************************
*                                              内部函数实现
*********************************************************************************************************/
static void InitUARTBuf(void)//初始化串口缓冲区，包括发送缓冲区和接收缓冲区
{
  i16 i;
  
  for(i=0;i<UART1_BUF_SIZE;i++)
  {
    s_arrRecdBuf[i]=0;
    s_arrSendBuf[i]=0;
  }
  InitQueue(&s_structUARTSendCirQue,s_arrSendBuf,UART1_BUF_SIZE);
  InitQueue(&s_structUARTRecCirQue,s_arrRecdBuf,UART1_BUF_SIZE);
}

static u8 WriteReceiveBuf(u8 d)//将接收缓冲区的数据写到变量d中
{
  u8 ok=0;//写入数据成功标志，0——不成功，1——成功
  ok=EnQueue(&s_structUARTRecCirQue,&d,1);
  return ok;//返回写入数据成功标志，0——不成功，1——成功
}

static u8 ReadSendBuf(u8 *p)//读取发送缓冲区中的数据
{
  u8 ok=0;//读取数据成功标志，0——不成功，1——成功
  ok=DeQueue(&s_structUARTSendCirQue,p,1);
  return ok;//返回读取数据成功标志，0——不成功，1——成功
}

static void ConfigUART(u32 bound)//配置串口相关参数
{
  GPIO_InitTypeDef GPIO_InitStructure;   //GPIO_InitStructure用于存放GPIO的参数
  USART_InitTypeDef USART_InitStructure; //USART_InitStructure用于存放USART的参数
  NVIC_InitTypeDef NVIC_InitStructure;   //NVIC_InitStructure用于存放NVIC的参数
  
  //使能RCC相关时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//使能USART1时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); //使能GPIOA时钟
  
  //配置TX的GPIO（输出）
  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;         //设置TX的引脚
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;   //设置TX的模式（复用推挽输出模式）
  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz; //设置TX的IO口输出速度
  GPIO_Init(GPIOA,&GPIO_InitStructure);           //根据参数初始化TX的GPIO
  
  //配置RX的GPIO（接收）
  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;            //设置RX的引脚
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING; //设置RX的模式（浮空输入模式）
  GPIO_Init(GPIOA,&GPIO_InitStructure);               //根据参数初始化RX的GPIO
  
  //配置USART的参数
  USART_StructInit(&USART_InitStructure);//初始化USART_InitStructure
  USART_InitStructure.USART_BaudRate=bound;//设置波特率
  USART_InitStructure.USART_WordLength=USART_WordLength_8b;//设置数据字长度
  USART_InitStructure.USART_StopBits=USART_StopBits_1;//设置停止位
  USART_InitStructure.USART_Parity=USART_Parity_No;//设置奇偶校验位
  USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;//设置模式（收发全开）
  USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;//设置硬件流控制模式
  USART_Init(USART1,&USART_InitStructure);//根据参数初始化USART1
  
  //配置NVIC
  NVIC_InitStructure.NVIC_IRQChannel=USART1_IRQn;//中断通道号
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//设置抢占优先级
  NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;//设置子优先级
  NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;//使能中断
  NVIC_Init(&NVIC_InitStructure);//根据参数初始化NVIC
  
  //使能USART1及其中断
  USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//使能接收非空中断
  USART_ITConfig(USART1,USART_IT_TXE,ENABLE);//使能发送寄存器空中断
  USART_Cmd(USART1,ENABLE);//使能USART1
  
  s_iUARTTxSts=UART_STATE_OFF;//串口发送数据状态设置为未发送数据
}

static void EnableUARTTx(void)//使能串口发送
{
  s_iUARTTxSts=UART_STATE_ON;//串口发送数据状态设置为正在发送数据
  USART_ITConfig(USART1,USART_IT_TXE,ENABLE);//使能发送中断
}


static void SendCharUsedByFputc(u16 ch)//发送字符函数，专由fputc函数调用
{
  ////USART_SendData将指定数据写入串口硬件 DR 寄存器，启动硬件串口发送
  USART_SendData(USART1,(u8)ch);//ch=uData,是一个存储一字节的变量
  
  //等待发送完毕
  while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET)
  {
    //TC是发送完成标志位，意思是当前字节还没全部发送完成，硬件还在传输中
    //其他标志位：
    //USART_FLAG_ORE:溢出错误：旧数据没读，新数据又来了，新字节丢失
    //USART_FLAG_TXE:发送数据寄存器 DR 为空，可以写入下一个待发字节
    //USART_FLAG_RXNE:收到新数据，DR 寄存器有字节可读    
  }  
}

//中断服务函数
void USART1_IRQHandler(void)
{
  u8 uData=0;
  
  //************************收到新的数据通知中断服务函数处理*********************************//
  if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET)//接收缓冲区非空中断
  {
    NVIC_ClearPendingIRQ(USART1_IRQn);//清除USART1中断挂起
    //读取 USART1 的 DR 寄存器里的接收数据，返回这个 8 位数据到uData，自动把 RXNE 标志清零
    uData=USART_ReceiveData(USART1);
    
    WriteReceiveBuf(uData);//将接收到的数据存入缓冲区（自己写的数组，相当于收纳筐）
   }
  
   //**********************接收缓冲区空了通知中断服务函数处理*******************************//
   if(USART_GetITStatus(USART1,USART_IT_TXE)!=RESET)//发送缓冲区空中断
  {
    NVIC_ClearPendingIRQ(USART1_IRQn);//清除USART1中断挂起
    USART_ClearITPendingBit(USART1,USART_IT_TXE);//清除发送中断标志
    
    ReadSendBuf(&uData);//从环形队列头部取出 1 个字节，存入变量uData
    
    USART_SendData(USART1,uData); //把uData写入USART_DR,同时硬件会自动把TXE清零
    
    if(QueueEmpty(&s_structUARTSendCirQue))//当发送缓冲区为空时
    {
      s_iUARTTxSts=UART_STATE_OFF;//串口发送数据状态设置为未发送数据
      USART_ITConfig(USART1,USART_IT_TXE,DISABLE);//关闭串口发送缓冲区空中断
    }
  }
 
   if(USART_GetFlagStatus(USART1,USART_FLAG_ORE)==SET)//判断有没有出现数据溢出错误，溢出错误标志为1
  {
    USART_ClearFlag(USART1,USART_FLAG_ORE);//清除溢出错误标志
    USART_ReceiveData(USART1);//读取USART_DR
  } 
}
/*********************************************************************************************************
*                                              API函数实现
*********************************************************************************************************/
void InitUART1(u32 bound)
{
  InitUARTBuf();
  
  ConfigUART(bound);
}

u8 WriteUART1(u8 *pBuf,u8 len)
{
  u8 wLen=0;
  
  wLen=EnQueue(&s_structUARTSendCirQue,pBuf,len);
  
  if(wLen<UART1_BUF_SIZE)
  {
    if(s_iUARTTxSts==UART_STATE_OFF)
    {
      EnableUARTTx();
    }
  }
  return wLen;
}

u8 ReadUART1(u8 *pBuf,u8 len)
{
  u8 rLen=0;
  rLen=DeQueue(&s_structUARTRecCirQue,pBuf,len);
  return rLen;
}

int fputc(int ch,FILE* f)
{
  SendCharUsedByFputc((u8)ch);
  
  return ch;
}
