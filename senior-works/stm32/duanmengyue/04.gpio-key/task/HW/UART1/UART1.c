/*********************************************************************************************************
* 模块名称: UART1.c
* 摘    要: 串口模块，包括串口模块初始化，以及中断服务函数处理，以及读写串口函数实现
* 当前版本: 1.0.0
* 作    者: SZLY(COPYRIGHT 2018 SZLY. All rights reserved.)
* 完成日期: 2018年01月01日 
* 内    容:
* 注    意: none                                                                  
**********************************************************************************************************
* 取代版本: 
* 作    者:
* 完成日期: 
* 修改内容:
* 修改文件: 
*********************************************************************************************************/

/*********************************************************************************************************
*                                              包含头文件
*********************************************************************************************************/
#include "stm32f10x_conf.h"
#include "UART1.h"
#include "Queue.h"

/*********************************************************************************************************
*                                              宏定义
*********************************************************************************************************/

/*********************************************************************************************************
*                                              枚举结构体定义
*********************************************************************************************************/
//工作模式
typedef enum
{
  UART_STATE_OFF, //串口关闭
  UART_STATE_ON,  //串口打开
  UART_STATE_MAX
}EnumUARTState;             

/*********************************************************************************************************
*                                              内部变量
*********************************************************************************************************/    
static  StructCirQue s_structUARTSendCirQue;  //发送串口CirQue
static  StructCirQue s_structUARTRecCirQue;   //接收串口CirQue
static  u8  s_arrSendBuf[UART1_BUF_SIZE];     //发送串口Buf
static  u8  s_arrRecBuf[UART1_BUF_SIZE];      //接收串口Buf

static  u8  s_iUARTTxSts;                     //USART.TX状态标志
          
/*********************************************************************************************************
*                                              内部函数声明
*********************************************************************************************************/
static  void  InitUARTBuf(void);      //初始化串口缓冲区，包括发送缓冲区和接收缓冲区 
static  u8    WriteReceiveBuf(u8 d);  //将接收到的数据写入到接收缓冲区中
static  u8    ReadSendBuf(u8 *p);     //读取发送缓冲区中的数据
                                            
static  void  ConfigUART(u32 bound);  //配置UART的参数，包括GPIO、RCC、USART常规参数和NVIC 
static  void  EnableUARTTx(void);     //使能串口发送，在WriteUARTx中调用，即每次发送数据之后需要调用
                                      //打开TXE的中断开关
                                            
static  void  SendCharUsedByFputc(u16 ch);  //发送字符函数，专由fputc函数调用
  
/*********************************************************************************************************
*                                              内部函数实现
*********************************************************************************************************/
/*********************************************************************************************************
* 函数名称: InitUARTBuf
* 函数功能: 初始化串口缓冲区，包括发送缓冲区和接收缓冲区  
* 输入参数: void
* 输出参数: void
* 返 回 值: void 
* 创建日期: 2018年01月01日
* 注    意: 
*********************************************************************************************************/
static  void  InitUARTBuf(void)
{
  i16 i;

  for(i = 0; i < UART1_BUF_SIZE; i++)
  {
    s_arrSendBuf[i] = 0;
    s_arrRecBuf[i]  = 0;  
  }

  InitQueue(&s_structUARTSendCirQue, s_arrSendBuf, UART1_BUF_SIZE);
  InitQueue(&s_structUARTRecCirQue,  s_arrRecBuf,  UART1_BUF_SIZE);
}
/*********************************************************************************************************
* 函数名称: WriteReceiveBuf
* 函数功能: 写数据到串口接收BUF，在中断服务函数中，当接收到数据时，将接收到的数据写入到串口接收BUF中 
* 输入参数: d，串口中断服务函数接收到的数据
* 输出参数: void
* 返 回 值: 写入数据成功标志，0-不成功，1-成功 
* 创建日期: 2018年01月01日
* 注    意: 
*********************************************************************************************************/
static  u8  WriteReceiveBuf(u8 d)
{
  u8 ok = 0;  //写入数据成功标志，0-不成功，1-成功
                                                                    
  ok = EnQueue(&s_structUARTRecCirQue, &d, 1);   
                                                                    
  return ok;  //返回写入数据成功标志，0-不成功，1-成功 
}

/*********************************************************************************************************
* 函数名称: ReadSendBuf
* 函数功能: 读取发送BUF，用户通过WriteUart将要写入的数据传至发送缓冲，中断函数通过接收缓冲读出 
* 输入参数: p，读出来的数据存放的首地址
* 输出参数: p，读出来的数据存放的首地址
* 返 回 值: 读取数据成功标志，0-不成功，1-成功 
* 创建日期: 2018年01月01日
* 注    意: 
*********************************************************************************************************/
static  u8  ReadSendBuf(u8 *p)
{
  u8 ok = 0;  //读取数据成功标志，0-不成功，1-成功
                                                                   
  ok = DeQueue(&s_structUARTSendCirQue, p, 1);  
                                                                   
  return ok;  //返回读取数据成功标志，0-不成功，1-成功 
}

/*********************************************************************************************************
* 函数名称: ConfigUART
* 函数功能: 配置UART的参数，包括GPIO、RCC、参数和NVIC  
* 输入参数: bound,波特率
* 输出参数: void
* 返 回 值: void
* 创建日期: 2018年01月01日
* 注    意: 
*********************************************************************************************************/
static  void  ConfigUART(u32 bound)
{
  GPIO_InitTypeDef  GPIO_InitStructure;      //定义结构体GPIO_InitStructure，用来配置USART的GPIO
  USART_InitTypeDef USART_InitStructure;     //定义结构体USART_InitStructure，用来配置USART的常规参数
  NVIC_InitTypeDef  NVIC_InitStructure;      //定义结构体NVIC_InitStructure，用来配置USART的NVIC

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);    //开启USART的时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);     //开启USART的GPIO时钟

  
  //配置USART.Tx的GPIO
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;               //设置USART.TX的引脚
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;          //设置USART.TX的模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;         //设置USART.TX的I/O口速率
  GPIO_Init(GPIOA, &GPIO_InitStructure);                    //根据参数初始化USART.TX的GPIO端口
  
  //配置USART.Rx的GPIO
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;              //设置USART.RX的引脚
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;    //设置USART.RX的模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;         //设置USART.RX的I/O口速率
  GPIO_Init(GPIOA, &GPIO_InitStructure);                    //根据参数初始化USART.RX的GPIO端口
  
  //配置USART的常规参数
  USART_StructInit(&USART_InitStructure);                   //初始化串口的各项配置
  USART_InitStructure.USART_BaudRate   = bound;             //设置USART的波特率
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;   //设置USART的串口传输的字长
  USART_InitStructure.USART_StopBits   = USART_StopBits_1;  //设置USART的停止位
  USART_InitStructure.USART_Parity     = USART_Parity_No;   //设置USART的奇偶校验位
  USART_InitStructure.USART_Mode       = USART_Mode_Rx | USART_Mode_Tx;           //设置USART的模式
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //设置USART的硬件流控制
  USART_Init(USART1, &USART_InitStructure);                 //根据参数初始化USART

  //配置USART的NVIC
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;         //开启USART的中断
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //抢占优先级，屏蔽即默认值
  NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 1; //子优先级，屏蔽即默认值
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //IRQ通道使能
  NVIC_Init(&NVIC_InitStructure);                           //根据参数初始化USART的NVIC寄存器

  //打开串口使能开关
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);  //接收中断使能
  USART_ITConfig(USART1, USART_IT_TXE,  ENABLE);  //发送中断使能
  USART_Cmd(USART1, ENABLE);                      //使能USART
                                                                     
  s_iUARTTxSts = UART_STATE_OFF;  //初始化USART.TX状态标志
}

/*********************************************************************************************************
* 函数名称: EnableUARTTx
* 函数功能: 使能串口发送，在WriteUARTx中调用，即每次发送数据之后需要调用这个函数来打开TXE的中断开关 
* 输入参数: void
* 输出参数: void
* 返 回 值: void 
* 创建日期: 2018年01月01日
* 注    意: s_iUARTTxSts = UART_STATE_ON;这句话必须放在USART_ITConfig之前，否则会导致中断打开无法执行
*********************************************************************************************************/
static  void  EnableUARTTx(void)
{
  s_iUARTTxSts = UART_STATE_ON;                     //将s_iUARTTxSts的状态置为打开

  USART_ITConfig(USART1, USART_IT_TXE, ENABLE);     //发送中断使能

}

/*********************************************************************************************************
* 函数名称: SendCharUsedByFputc
* 函数功能: 发送字符函数，专由fputc函数调用  
* 输入参数: ch，待发送的字符
* 输出参数: void
* 返 回 值: void 
* 创建日期: 2018年01月01日
* 注    意: 
*********************************************************************************************************/
static  void  SendCharUsedByFputc(u16 ch)
{  
  USART_SendData(USART1, (u8)ch);//发送一个字节数据到串口

  //等待发送完毕
  while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
  {
    
  }
}

/*********************************************************************************************************
* 函数名称: USART1_IRQHandler
* 函数功能: USART1中断服务函数 
* 输入参数: void
* 输出参数: void
* 返 回 值: void
* 创建日期: 2018年01月01日
* 注    意: 
*********************************************************************************************************/
void USART1_IRQHandler(void)                
{
  u8  uData = 0;

  if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) //接收中断
  {
    NVIC_ClearPendingIRQ(USART1_IRQn);  //清除接收中断
    uData = USART_ReceiveData(USART1);  //接收串口传来的数据，并存到uData中    
    WriteReceiveBuf(uData); //当接收到数据时，将接收到的数据写入到接收缓冲区中
  }
  
  if(USART_GetFlagStatus(USART1, USART_FLAG_ORE) == SET)  //如果USART溢出错误标志位为“1”
  {
    USART_ClearFlag(USART1, USART_FLAG_ORE);  //清除USART的溢出错误标志
    USART_ReceiveData(USART1);  //从USART接收数据 
  }
  
  if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)  //发送中断
  {
    USART_ClearITPendingBit(USART1, USART_IT_TXE);  //清除USART的发送中断标志位
    NVIC_ClearPendingIRQ(USART1_IRQn);  //清除发送中断
    
    ReadSendBuf(&uData);  //读取发送缓冲区的数据到uData
    
    USART_SendData(USART1, uData);  //串口发送数据uData
                                                                                           
    if(QueueEmpty(&s_structUARTSendCirQue)) //当发送缓冲区数据长度为0
    {                                                               
      s_iUARTTxSts = UART_STATE_OFF;  //USART.TX状态标志设置为关闭       
      USART_ITConfig(USART1, USART_IT_TXE, DISABLE);  //关闭串口发送中断
    }
  }
}  

/*********************************************************************************************************
*                                              API函数实现
*********************************************************************************************************/
/*********************************************************************************************************
* 函数名称: InitUART1
* 函数功能: 初始化UART模块 
* 输入参数: bound,波特率
* 输出参数: void
* 返 回 值: void
* 创建日期: 2018年01月01日
* 注    意: 
*********************************************************************************************************/
void InitUART1(u32 bound)
{
  InitUARTBuf();  //初始化串口缓冲区，包括发送缓冲区和接收缓冲区  
                  
  ConfigUART(bound);  //配置UART的参数，包括GPIO、RCC、常规参数和NVIC
}

/*********************************************************************************************************
* 函数名称: WriteUART1
* 函数功能: 写数据到相应的串口  
* 输入参数: pBuf，要写入数据的首地址，len，要写入数据的长度
* 输出参数: void
* 返 回 值: 成功写入数据的个数，不一定与l输入参数len相等
* 创建日期: 2018年01月01日
* 注    意: 
*********************************************************************************************************/
u8  WriteUART1(u8 *pBuf, u8 len)
{
  u8 wLen = 0;  //实际写入数据的长度
                                                                  
  wLen = EnQueue(&s_structUARTSendCirQue, pBuf, len);

  if(wLen < UART1_BUF_SIZE)
  {
    if(s_iUARTTxSts == UART_STATE_OFF)
    {
      EnableUARTTx();
    }    
  }
                                                                  
  return wLen;  //返回实际写入数据的长度
}

/*********************************************************************************************************
* 函数名称: ReadUART1
* 函数功能: 读取相应串口BUF中的数据  
* 输入参数: len，欲读取数据的个数
* 输出参数: pBuf，读取的数据存放的首地址
* 返 回 值: 成功读取数据的个数
* 创建日期: 2018年01月01日
* 注    意: 
*********************************************************************************************************/
u8  ReadUART1(u8 *pBuf, u8 len)
{
  u8 rLen = 0;  //实际读取数据长度
                                                    
  rLen = DeQueue(&s_structUARTRecCirQue, pBuf, len);

  return rLen;  //返回实际读取数据的长度
}

/*********************************************************************************************************
* 函数名称: fputc
* 函数功能: 重定向函数  
* 输入参数: ch，f
* 输出参数: void
* 返 回 值: int 
* 创建日期: 2018年01月01日
* 注    意: 
*********************************************************************************************************/
int fputc(int ch, FILE* f)
{
  SendCharUsedByFputc((u8) ch);  //发送字符函数，专由fputc函数调用
                               
  return ch;                     //返回ch
}
