/*********************************************************************************************************
* 模块名称：Capture.c
* 摘    要：Capture模块
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
#include "Capture.h"
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
//s_iCaptureSts中的bit7为捕获完成的标志，bit6为捕获到下降沿标志，bit5-bit0为捕获到下降沿后定时器溢出的次数
static  u8  s_iCaptureSts = 0;     //捕获状态 
static  u16 s_iCaptureVal;         //捕获值
u8 TenTimes_count=0;
u32 sum=0;
float captureVal=0.0f;////float单精度浮点，可以保存小数点
/*********************************************************************************************************
*                                              内部函数声明
*********************************************************************************************************/
static  void ConfigTIM5ForCapture(u16 arr, u16 psc);    //配置TIM5
/*********************************************************************************************************
*                                              内部函数实现
*********************************************************************************************************/
/*********************************************************************************************************
* 函数名称：ConfigTIM5ForCapture
* 函数功能：配置TIM3 
* 输入参数：arr-自动重装值，psc-预分频器值
* 输出参数：void
* 返 回 值：void
* 创建日期：2018年01月01日 
* 注    意：此处暂定使用定时器TIM5的CH2（PA1）来做输入捕获，捕获PB5,TIM_CH2上高电平的脉宽时间
*********************************************************************************************************/
static  void ConfigTIM5ForCapture(u16 arr, u16 psc)
{ 
  GPIO_InitTypeDef        GPIO_InitStructure;    //GPIO_InitStructure用于存放GPIO的参数
  TIM_TimeBaseInitTypeDef TIMx_TimeBaseStructure;//TIM_TimeBaseStructure用于存放定时器的基本参数
  TIM_ICInitTypeDef       TIMx_ICInitStructure;  //TIMx_ICInitStructure用于存放定时器的通道参数
  NVIC_InitTypeDef        NVIC_InitStructure;    //NVIC_InitStructure用于存放NVIC的参数

  //使能RCC相关时钟
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);  //使能TIM5的时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //使能捕获的GPIOA的时钟
  
  
  //配置PA1，对应TIM5的CH2
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1;            //设置引脚
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;         //设置输入模式 
  GPIO_Init(GPIOA, &GPIO_InitStructure);                //根据参数初始化GPIO
  GPIO_SetBits(GPIOA, GPIO_Pin_1);  //将捕获对应的引脚置为高电平
  
  //配置TIM5
  TIMx_TimeBaseStructure.TIM_Period        = arr;                     //设定计数器自动重装值 
  TIMx_TimeBaseStructure.TIM_Prescaler     = psc;                     //设置TIMx时钟频率除数的预分频值  
  TIMx_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;            //设置时钟分割
  TIMx_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;      //设置定时器TIMx为向上计数模式
  TIM_TimeBaseInit(TIM5, &TIMx_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
  
  //配置TIM5的CH2为输入捕获
  //CC1S = 01，CC2通道被配置为输入，输入通道IC2映射到定时器引脚TI1上
  TIMx_ICInitStructure.TIM_Channel        = TIM_Channel_2;            //设置输入通道通道 
  TIMx_ICInitStructure.TIM_ICPolarity     = TIM_ICPolarity_Rising;   //设置为上升沿捕获
  TIMx_ICInitStructure.TIM_ICSelection    = TIM_ICSelection_DirectTI; //设置为直接映射到TI2(PA1)
  TIMx_ICInitStructure.TIM_ICPrescaler    = TIM_ICPSC_DIV1;           //设置为每一个边沿都捕获，捕捉不分频 
  TIMx_ICInitStructure.TIM_ICFilter       = 0x08;                     //设置输入滤波器
  TIM_ICInit(TIM5, &TIMx_ICInitStructure);         //根据参数初始化TIM5的CH2
  
  //配置NVIC
  NVIC_InitStructure.NVIC_IRQChannel                   = TIM5_IRQn;     //中断通道号   
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;             //设置抢占优先级   
  NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;             //设置子优先级   
  NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;        //使能中断   
  NVIC_Init(&NVIC_InitStructure);                                       //根据参数初始化NVIC    
  
  TIM_ITConfig(TIM5, TIM_IT_Update | TIM_IT_CC2, ENABLE); //使能定时器的更新中断和CC1IE捕获中断 
  
  TIM_Cmd(TIM5, ENABLE);  //使能TIM5
}

/*********************************************************************************************************
* 函数名称：TIM5_IRQHandler
* 函数功能：TIM5中断服务函数
* 输入参数：void
* 输出参数：void
* 返 回 值：void
* 创建日期：2018年01月01日 
* 注    意：
*********************************************************************************************************/
void TIM5_IRQHandler(void)
{ 
  if((s_iCaptureSts & 0x80) == 0) //bit7=0：单次捕获流程未完成
  {  
    //定时器溢出中断：高电平持续时间过长时累加溢出次数
    if(TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)        
    {    
      if(s_iCaptureSts & 0x40)    //bit6=1：已捕获上升沿，正在计时中发生溢出
      {
        if((s_iCaptureSts & 0x3F) == 0x3F) //溢出次数达到上限，强制结束捕获
        {
          s_iCaptureSts |= 0x80;
          s_iCaptureVal = 0xFFFF;
        } 
        else
        {
          s_iCaptureSts++; //溢出次数+1
        }
      }
    }
    
    //CH2捕获边沿中断
    if (TIM_GetITStatus(TIM5, TIM_IT_CC2) != RESET) 
    { 
      if(s_iCaptureSts & 0x40)  
      {
        //场景：已捕获上升沿，本次捕获到下降沿 → 高电平结束，读取脉宽
        s_iCaptureSts |= 0x80;  //标记单次捕获完成
        s_iCaptureVal = TIM_GetCapture2(TIM5);
        TIM_OC2PolarityConfig(TIM5, TIM_ICPolarity_Rising); //切回上升沿，等待下一轮波形
      }
      else  
      {  
        //场景：首次捕获上升沿 → 高电平开始，清零计数器启动计时
        s_iCaptureSts = 0;
        s_iCaptureVal = 0;
        TIM_SetCounter(TIM5, 0);
        s_iCaptureSts |= 0x40;    //bit6置1：标记已捕获上升沿
        TIM_OC2PolarityConfig(TIM5, TIM_ICPolarity_Falling); //切换捕获下降沿，等待高电平结束
      }    
    } 
  }
  //清除中断标志
  TIM_ClearITPendingBit(TIM5, TIM_IT_CC2 | TIM_IT_Update);    
}
/*********************************************************************************************************
*                                              API函数实现
*********************************************************************************************************/
/*********************************************************************************************************
* 函数名称：InitCapture
* 函数功能：初始化Capture模块 
* 输入参数：void
* 输出参数：void
* 返 回 值：void
* 创建日期：2018年01月01日 
* 注    意：
*********************************************************************************************************/
void  InitCapture(void)
{
  //计数器达到最大装载值0xFFFF，会产生溢出；以72MHz/（72-1+1）=1MHz的频率计数，即1us
  ConfigTIM5ForCapture(0xFFFF, 72 - 1);  
}

/*********************************************************************************************************
* 函数名称：GetCaptureVal
* 函数功能：获取捕获时间，返回值为1表示捕获成功，此时*pCapVal才有意义 
* 输入参数：void
* 输出参数：pCalVal，捕获到的值的地址
* 返 回 值：ok：1-获取成功
* 创建日期：2018年01月01日 
* 注    意：
*********************************************************************************************************/
u8   GetCaptureVal(i32* pCapVal)
{
  u8  ok = 0;
  
  if(s_iCaptureSts & 0x80)              //最高位为1，表示成功捕获到了上升沿（获取到按键弹起标志）
  {
    (*pCapVal)  = s_iCaptureSts & 0x3F; //取出低6位计数器的值赋给(*pCapVal)，得到溢出次数
    (*pCapVal) *= 65536;//计数器计数次数为2^16=65536，乘以溢出次数，得到溢出时间总和（以1/1MHz=1us为单位）
    (*pCapVal) += s_iCaptureVal;        //加上最后一次比较捕获寄存器的值，得到单次脉宽

    s_iCaptureSts = 0;                  //设置为0，开启下一次捕获
    
    TenTimes_count++;//表示记录到了一次高电平持续时间，需要积累10次
    sum+=(*pCapVal);
    
    if(TenTimes_count>=10)
    {
      ok=1;
      TenTimes_count=0;
      captureVal = sum/10000.0;
      sum=0;
    }
  }

  return(ok);                           //返回是否捕获成功的标志
}
