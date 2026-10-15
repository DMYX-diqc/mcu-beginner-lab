/*********************************************************************************************************
* 模块名称：EXTI.c
* 摘    要：EXTI模块
* 当前版本：1.0.0
* 作    者：SZLY(COPYRIGHT 2018 - 2020 SZLY. All rights reserved.)
* 完成日期：2020年01月01日
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
#include "EXTI.h"
#include "stm32f10x_conf.h"

/*********************************************************************************************************
*                                              宏定义
*********************************************************************************************************/

/*********************************************************************************************************
*                                              枚举结构体定义
*********************************************************************************************************/

/*********************************************************************************************************
*                                              内部变量
*********************************************************************************************************/

/*********************************************************************************************************
*                                              内部函数声明
*********************************************************************************************************/
static void ConfigEXTIGPIO(void);   //配置EXTI的GPIO 
static void ConfigEXTI(void);       //配置EXTI

/*********************************************************************************************************
*                                              内部函数实现
*********************************************************************************************************/
/*********************************************************************************************************
* 函数名称：ConfigEXTIGPIO
* 函数功能：配置EXTI的GPIO 
* 输入参数：void 
* 输出参数：void
* 返 回 值：void
* 创建日期：2018年01月01日
* 注    意：
*********************************************************************************************************/
static void ConfigEXTIGPIO(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;  //GPIO_InitStructure用于存放GPIO的参数
                                                                     
  //使能RCC相关时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); //使能GPIOC的时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //使能GPIOA的时钟
  
  //配置PC1
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1;           //设置引脚
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;        //设置输入类型
  GPIO_Init(GPIOC, &GPIO_InitStructure);                //根据参数初始化GPIO
                                                                                          
  //配置PC2
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;           //设置引脚
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;        //设置输入类型
  GPIO_Init(GPIOC, &GPIO_InitStructure);                //根据参数初始化GPIO
                                                                                       
  //配置PA0
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0;           //设置引脚
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;        //设置输入类型
  GPIO_Init(GPIOA, &GPIO_InitStructure);                //根据参数初始化GPIO
}

/*********************************************************************************************************
* 函数名称：ConfigEXTI
* 函数功能：配置EXTI
* 输入参数：void 
* 输出参数：void
* 返 回 值：void
* 创建日期：2018年01月01日
* 注    意：
*********************************************************************************************************/
static void ConfigEXTI(void)
{
  EXTI_InitTypeDef EXTI_InitStructure;  //EXTI_InitStructure用于存放EXTI的参数
  NVIC_InitTypeDef NVIC_InitStructure;  //NVIC_InitStructure用于存放NVIC的参数
  
  //使能RCC相关时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);  //使能AFIO的时钟
  
  //配置PC1的EXTI和NVIC 
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource1);   //选择引脚作为中断线
  EXTI_InitStructure.EXTI_Line = EXTI_Line1;                    //选择中断线
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;           //开放中断请求
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;        //设置为上升沿触发
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;                     //使能中断线 
  EXTI_Init(&EXTI_InitStructure);                               //根据参数初始化EXTI
  
  NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;              //中断通道号
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;     //设置抢占优先级
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;            //设置子优先级
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;               //使能中断
  NVIC_Init(&NVIC_InitStructure);                               //根据参数初始化NVIC
  
  //配置PC2的EXTI和NVIC  
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource2);   //选择引脚作为中断线
  EXTI_InitStructure.EXTI_Line = EXTI_Line2;                    //选择中断线
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;           //开放中断请求
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;        //设置为上升沿触发
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;                     //使能中断线 
  EXTI_Init(&EXTI_InitStructure);                               //根据参数初始化EXTI
  
  NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;              //中断通道号
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;     //设置抢占优先级
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;            //设置子优先级
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;               //使能中断
  NVIC_Init(&NVIC_InitStructure);                               //根据参数初始化NVIC
  
  //配置PA0的EXTI和NVIC
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);   //选择引脚作为中断线
  EXTI_InitStructure.EXTI_Line = EXTI_Line0;                    //选择中断线
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;           //开放中断请求
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;        //设置为上升沿触发
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;                     //使能中断线 
  EXTI_Init(&EXTI_InitStructure);                               //根据参数初始化EXTI
  
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;              //中断通道号
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;     //设置抢占优先级
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;            //设置子优先级
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;               //使能中断
  NVIC_Init(&NVIC_InitStructure);                               //根据参数初始化NVIC
}

/*********************************************************************************************************
* 函数名称：EXTI0_IRQHandler
* 函数功能：EXTI2的中断服务函数，对应KEY3
* 输入参数：void 
* 输出参数：void
* 返 回 值：void
* 创建日期：2018年01月01日
* 注    意：
*********************************************************************************************************/
void EXTI0_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line0) != RESET) //判断中断是否发生
  {   
    //LED1状态取反
    GPIO_WriteBit(GPIOC, GPIO_Pin_4, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_4)));
    //LED2状态取反
    GPIO_WriteBit(GPIOC, GPIO_Pin_5, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_5)));    
    EXTI_ClearITPendingBit(EXTI_Line0);     //清除Line0上的中断标志位
  }
}

/*********************************************************************************************************
* 函数名称：EXTI1_IRQHandler
* 函数功能：EXTI2的中断服务函数，对应KEY1
* 输入参数：void 
* 输出参数：void
* 返 回 值：void
* 创建日期：2018年01月01日
* 注    意：
*********************************************************************************************************/
void EXTI1_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line1) != RESET) //判断中断是否发生
  {
    //LED1状态取反
    GPIO_WriteBit(GPIOC, GPIO_Pin_4, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_4)));    
    EXTI_ClearITPendingBit(EXTI_Line1);     //清除Line1上的中断标志位
  }
}

/*********************************************************************************************************
* 函数名称：EXTI2_IRQHandler
* 函数功能：EXTI2的中断服务函数，对应KEY2
* 输入参数：void 
* 输出参数：void
* 返 回 值：void
* 创建日期：2018年01月01日
* 注    意：
*********************************************************************************************************/
void EXTI2_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line2) != RESET) //判断中断是否发生
  {
    //LED2状态取反
    GPIO_WriteBit(GPIOC, GPIO_Pin_5, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_5)));
    EXTI_ClearITPendingBit(EXTI_Line2);     //清除Line2上的中断标志位
  }
}

/*********************************************************************************************************
*                                              API函数实现
*********************************************************************************************************/
/*********************************************************************************************************
* 函数名称：InitEXTI
* 函数功能：初始化EXTI模块
* 输入参数：void 
* 输出参数：void
* 返 回 值：void
* 创建日期：2018年1月1日
* 注    意：
*********************************************************************************************************/
void  InitEXTI(void)
{
  ConfigEXTIGPIO(); //配置EXTI的GPIO
  ConfigEXTI();     //配置EXTI
}
