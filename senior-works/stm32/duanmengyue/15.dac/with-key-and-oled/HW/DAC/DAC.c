/*********************************************************************************************************
* 模块名称：DAC.c
* 摘    要：DAC模块
* 当前版本：1.0.0
* 作    者：SZLY(COPYRIGHT 2018 - 2020 SZLY. All rights reserved.)
* 完成日期：2020年01月01日 
* 内    容：
* 注    意： DAC2——PA5，对应的DMA是DMA2_CH4                                                                
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
#include "DAC.h"
#include "Wave.h"
#include "stm32f10x_conf.h"
/*********************************************************************************************************
*                                              宏定义
*********************************************************************************************************/
#define DAC_DHR12R2_ADDR    ((u32)0x40007414)   //DAC2的地址（12位右对齐）
/*********************************************************************************************************
*                                              枚举结构体定义
*********************************************************************************************************/

/*********************************************************************************************************
*                                              内部变量
*********************************************************************************************************/
static StructDACWave s_strDAC2WaveBuf;  //存储DAC2波形属性，包括波形地址和点数 
static u16 Amp_Scale = 100;       //幅值比例，默认100%，范围10~100，每次±10
static u16 Wave_Scaled_Buf[100];  //缩放数组，100个点，和波形表长度一致

// 定义三种波形结构体
StructDACWave sine_wave;//正弦
StructDACWave square_wave;//方波
StructDACWave tri_wave;//三角

static StructDACWave origin_wave; //存当前选中的原始波形（永远不变的原始数组）
/*********************************************************************************************************
*                                              内部函数声明
*********************************************************************************************************/
static  void ConfigTimer4(u16 arr, u16 psc);            //配置TIM4
static  void ConfigDAC2(void);                          //配置DAC1
static  void ConfigDMA2Ch4ForDAC2(StructDACWave wave);  //配置DMA2通道4
static void Scale_Wave_Data(const u16* src, u16 len, u16 scale);

/*********************************************************************************************************
*                                              内部函数实现
*********************************************************************************************************/
/*********************************************************************************************************
* 函数名称：ConfigTimer4
* 函数功能：配置TIM4
* 输入参数：arr-自动重装值，psc-预分频器值
* 输出参数：void
* 返 回 值：void
* 创建日期：2018年01月01日
* 注    意：DAC每两个点之间的时间差为(arr+1)*(psc+1)/72(us)，100个点，周期即为1ms
*********************************************************************************************************/
static  void ConfigTimer4(u16 arr, u16 psc)
{
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;   //TIM_TimeBaseStructure用于存放定时器的参数
  
  //使能RCC相关时钟
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);   //使能定时器的时钟
  
  //配置TIM4
  TIM_DeInit(TIM4);                                 //重置为默认值
  TIM_TimeBaseStructure.TIM_Period        = arr;    //设置自动重装载值   
  TIM_TimeBaseStructure.TIM_Prescaler     = psc;    //设置预分频器值    
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;        //设置时钟分割：tDTS = tCK_INT
  TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;  //设置向上计数模式 
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);                //根据参数初始化定时器

  TIM_SelectOutputTrigger(TIM4, TIM_TRGOSource_Update);          //选择更新事件为触发输入

  TIM_Cmd(TIM4, ENABLE);  //使能定时器
} 

/*********************************************************************************************************
* 函数名称：ConfigDAC2
* 函数功能：配置DAC2，DAC2通过PA5输出
* 输入参数：void
* 输出参数：void
* 返 回 值：void
* 创建日期：2018年01月01日
* 注    意：为避免寄生干扰和额外功耗，GPIO模式建议设置为模拟输入
*********************************************************************************************************/
static  void ConfigDAC2(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure; //GPIO_InitStructure用于存放GPIO的参数
  DAC_InitTypeDef   DAC_InitStructure;  //DAC_InitStructure用于存放DAC的参数

  //使能RCC相关时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //使能GPIOA的时钟
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);    //使能DAC的时钟
  
  //配置DAC2的GPIO
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_5;            //设置引脚
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;      //设置I/O输出速度
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AIN;         //设置输入类型
  GPIO_Init(GPIOA, &GPIO_InitStructure);                 //根据参数初始化GPIO
  
  //配置DAC2
  DAC_InitStructure.DAC_Trigger = DAC_Trigger_T4_TRGO;            //设置DAC触发
  DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None; //关闭波形发生器
  DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0; //不屏蔽LSFR位0/三角波幅值等于1
  DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;   //使能DAC输出缓存
  DAC_Init(DAC_Channel_2, &DAC_InitStructure);    //初始化DAC通道2

  DAC_DMACmd(DAC_Channel_2, ENABLE);              //使能DAC通道2的DMA模式
  
  DAC_SetChannel2Data(DAC_Align_12b_R, 0);        //设置为12位右对齐数据格式
  
  DAC_Cmd(DAC_Channel_2, ENABLE);                 //使能DAC通道2
}

/*********************************************************************************************************
* 函数名称：ConfigDMA2Ch4ForDAC2
* 函数功能：配置DMA2通道4，DMA搬运工
* 输入参数：wave，包括波形地址和点数
* 输出参数：void
* 返 回 值：void
* 创建日期：2018年01月01日
* 注    意：
*********************************************************************************************************/
static  void ConfigDMA2Ch4ForDAC2(StructDACWave wave)
{  
  DMA_InitTypeDef   DMA_InitStructure;  //DMA_InitStructure用于存放DMA的参数
  NVIC_InitTypeDef  NVIC_InitStructure; //NVIC_InitStructure用于存放NVIC的参数

  //使能RCC相关时钟
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);  //使能DMA2的时钟  
  
  //配置DMA2_Channel4
  DMA_DeInit(DMA2_Channel4);  //将DMA2_CH4寄存器设置为默认值
  DMA_InitStructure.DMA_PeripheralBaseAddr = DAC_DHR12R2_ADDR;     //设置外设地址
  DMA_InitStructure.DMA_MemoryBaseAddr     = wave.waveBufAddr;     //设置存储器地址
  DMA_InitStructure.DMA_BufferSize         = wave.waveBufSize;     //设置要传输的数据项数目                                              
  DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralDST;//设置为存储器到外设模式
  DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;  //设置外设为非递增模式
  DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;       //设置存储器为递增模式
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //设置外设数据长度为半字
  DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_HalfWord;	    //设置存储器数据长度为半字
  DMA_InitStructure.DMA_Mode               = DMA_Mode_Circular;    //设置为循环模式
  DMA_InitStructure.DMA_Priority           = DMA_Priority_High;    //设置为高优先级
  DMA_InitStructure.DMA_M2M                = DMA_M2M_Disable;      //禁止存储器到存储器访问
  DMA_Init(DMA2_Channel4, &DMA_InitStructure); //根据参数初始化DMA2_Channel4

  //配置NVIC
  NVIC_InitStructure.NVIC_IRQChannel = DMA2_Channel4_5_IRQn;   //中断通道号
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //设置抢占优先级
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;         //设置子优先级
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;            //使能中断
  NVIC_Init(&NVIC_InitStructure);                            //根据参数初始化NVIC 

  DMA_ITConfig(DMA2_Channel4, DMA_IT_TC, ENABLE);  //使能DMA2_Channel4的传输完成中断 

  DMA_Cmd(DMA2_Channel4, ENABLE);                  //使能DMA2_Channel4
}

/*********************************************************************************************************
* 函数名称：DMA2_Channel4_5_IRQHandler
* 函数功能：DMA2_Channel4的中断服务函数
* 输入参数：void
* 输出参数：void
* 返 回 值：void
* 创建日期：2018年01月01日
* 注    意：当定时器溢出（ARR）或者比较匹配时，发出TRGO触发次中断服务函数
*********************************************************************************************************/
void DMA2_Channel4_5_IRQHandler(void)
{
  if(DMA_GetITStatus(DMA2_IT_TC4))  //判断DMA2_Channel4传输完成中断是否发生
  {
    //外设标志清零 + 内核挂起清零，中断才算完整处理完毕。
    NVIC_ClearPendingIRQ(DMA2_Channel4_5_IRQn);  //清除DMA2_Channel4中断挂起
    DMA_ClearITPendingBit(DMA2_IT_GL4);        //清除DMA2_Channel4传输完成中断标志

    ConfigDMA2Ch4ForDAC2(s_strDAC2WaveBuf);    //配置DMA2通道4
  }
}

/*********************************************************************************************************
* 函数名称：Scale_Wave_Data
* 函数功能：根据比例缩放原始波形，结果存入缩放数组Wave_Scaled_Buf
* src：原始波形数组地址
* len：波形点数
* scale：缩放百分比（10~100）
*********************************************************************************************************/
static void Scale_Wave_Data(const u16* src, u16 len, u16 scale)
{
    u16 i;
    u32 temp;
    for(i = 0; i < len; i++)
    {
        // 原始数值 × 比例 ÷ 100，得到缩放后的DAC数值
        temp = (u32)src[i] * scale / 100;
        // DAC最大只能4095，防止溢出
        if(temp > 4095)
        {
            temp = 4095;
        }
        Wave_Scaled_Buf[i] = (u16)temp;
    }
}

/*********************************************************************************************************
*                                              API函数实现
*********************************************************************************************************/
/*********************************************************************************************************
* 函数名称：InitDAC
* 函数功能：初始化DAC模块 
* 输入参数：void
* 输出参数：void
* 返 回 值：void
* 创建日期：2018年01月01日
* 注    意：
*********************************************************************************************************/
void InitDAC(void)
{  
  // 1. 获取原始正弦波常量数组
  const u16* Sine_Buf = (const u16*)GetSineWave100PointAddr();
  // 2. 按默认100%比例缩放波形，存入缩放数组
  Scale_Wave_Data(Sine_Buf,100,Amp_Scale);
    
  s_strDAC2WaveBuf.waveBufAddr  = (u32)Wave_Scaled_Buf; //波形地址
  s_strDAC2WaveBuf.waveBufSize  = 100;                            //波形点数  

  ConfigDAC2(); //配置DAC2
  ConfigTimer4(799, 719);   //100KHz，计数一次是10us，计数到800溢出时间为8ms  
  ConfigDMA2Ch4ForDAC2(s_strDAC2WaveBuf); //配置DMA2通道4
  
  /*****************************新增三种波形初始化**********************************/
  sine_wave.waveBufAddr = (u32)GetSineWave100PointAddr();
  sine_wave.waveBufSize = 100;

  square_wave.waveBufAddr = (u32)GetRectWave100PointAddr();
  square_wave.waveBufSize = 100;

  tri_wave.waveBufAddr = (u32)GetTriWave100PointAddr();
  tri_wave.waveBufSize = 100;
  
  origin_wave.waveBufAddr = (u32)GetSineWave100PointAddr();
  origin_wave.waveBufSize = 100;

}

/*********************************************************************************************************
* 函数名称：SetDACWave
* 函数功能：设置DAC波形属性
* 输入参数：wave，包括波形地址和点数
* 输出参数：void
* 返 回 值：void
* 创建日期：2018年01月01日
* 注    意：
*********************************************************************************************************/
void SetDACWave(StructDACWave wave)
{
  // 把当前选中的原始波形存起来
  origin_wave.waveBufAddr = wave.waveBufAddr;
  origin_wave.waveBufSize = wave.waveBufSize;
  
  // 先把新波形按当前比例缩放，存入缩放数组
  Scale_Wave_Data((const u16*)wave.waveBufAddr, wave.waveBufSize, Amp_Scale);
  // 更新全局波形结构体，DMA读取新缩放数据
  s_strDAC2WaveBuf.waveBufAddr = (u32)Wave_Scaled_Buf;
  s_strDAC2WaveBuf.waveBufSize = wave.waveBufSize;
}


/*********************************************************************************************************
* 函数名称：DAC_Amp_Add
* 函数功能：增大波形幅值，KEY2调用，每次+10，最大100
*********************************************************************************************************/
void DAC_Amp_Add(void)
{
    if(Amp_Scale < 100)
    {
        Amp_Scale += 10;
        // 重新缩放当前正在输出的波形
        Scale_Wave_Data((const u16*)origin_wave.waveBufAddr,origin_wave.waveBufSize, Amp_Scale);
        // 重载DMA配置，新幅值立刻生效
        ConfigDMA2Ch4ForDAC2(s_strDAC2WaveBuf);
    }
}

/*********************************************************************************************************
* 函数名称：DAC_Amp_Sub
* 函数功能：减小波形幅值，KEY3调用，每次-10，最小10
*********************************************************************************************************/
void DAC_Amp_Sub(void)
{
    if(Amp_Scale > 10)
    {
        Amp_Scale -= 10;
        Scale_Wave_Data((const u16*)origin_wave.waveBufAddr, origin_wave.waveBufSize, Amp_Scale);
        ConfigDMA2Ch4ForDAC2(s_strDAC2WaveBuf);
    }
}
