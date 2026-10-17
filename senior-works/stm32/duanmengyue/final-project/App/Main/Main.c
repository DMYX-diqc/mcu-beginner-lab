/*********************************************************************************************************
* 模块名称：Main.c
* 摘    要：主文件，包含软硬件初始化函数和main函数
* 当前版本：1.0.0
* 作    者：SZLY(COPYRIGHT 2018 - 2020 SZLY. All rights reserved.)
* 完成日期：2020年01月01日
* 内    容：
* 注    意：注意勾选Options for Target 'Target1'->Code Generation->Use MicroLIB，否则printf无法使用                                                                  
***********************************************************************************************************/
#include "Main.h"
#include "stm32f10x_conf.h"
#include "DataType.h"
#include "NVIC.h"
#include "SysTick.h"
#include "RCC.h"
#include "Timer.h"
#include "LED.h"
#include "UART1.h"
#include "KeyOne.h"
#include "ProcKeyOne.h"
#include <string.h>
#include <ctype.h>
#include "Calculator.h"
#include "EXTI.h"
#include "OLED.h"
#include "IWDG.h"
//================================函数声明=================================//
static  void  InitSoftware(void);   //初始化软件相关的模块
static  void  InitHardware(void);   //初始化硬件相关的模块
static  void  Proc2msTask(void);    //2ms处理任务
static  void  Proc1SecTask(void);   //1s处理任务


/*********************************************************************************************************
* 函数名称：InitSoftware
* 函数功能：所有的软件相关的模块初始化函数都放在此函数中
* 输入参数：void
* 输出参数：void
* 返 回 值：void
* 创建日期：2018年01月01日
* 注    意：
*********************************************************************************************************/
static void  InitSoftware(void)
{
  InitCalculator(); //上电初始化清空计算器
}

/*********************************************************************************************************
* 函数名称：InitHardware
* 函数功能：所有的硬件相关的模块初始化函数都放在此函数中
* 输入参数：void
* 输出参数：void
* 返 回 值：void
* 创建日期：2018年01月01日
* 注    意：
*********************************************************************************************************/
static void  InitHardware(void)
{  
  SystemInit();       //系统初始化
  InitRCC();          //初始化RCC模块
  InitNVIC();         //初始化NVIC模块
  InitTimer();        //初始化Timer模块
  InitLED();          //初始化LED模块
  InitSysTick();      //初始化SysTick模块
  InitUART1(115200);
  InitProcKeyOne();
  InitKeyOne();
  InitEXTI();
  InitOLED(); 
  InitIWDG();
}

/*********************************************************************************************************
* 函数名称：Proc2msTask
* 函数功能：2ms处理任务 
* 输入参数：void
* 输出参数：void
* 返 回 值：void
* 创建日期：2018年01月01日
* 注    意：
*********************************************************************************************************/
static  void  Proc2msTask(void)
{  
  u8 recData;
  
  if(Get2msFlag())  //判断2ms标志状态
  { 
    LEDFlicker(250);//调用闪烁函数     
    
    if(ReadUART1(&recData,1))
    {
      // 合法字符：数字0~9、四则运算符；缓冲区未满才存入
        if( (isdigit(recData) || recData == '+' || recData == '-' || recData == '*' || recData == '/'|| recData=='(' || recData==')'|| recData == '.') 
           && expr_len < EXPR_MAX_LEN - 1 )          
        {
            expr_buf[expr_len++] = recData;
            expr_buf[expr_len] = '\0'; //字符串结束符
            flag_has_expr = 1; //标记存在有效表达式
        }
        // 回车/换行：提示已接收算式
        else if(recData == '\r' || recData == '\n')
        {
            printf("已接收算式：%s\r\n", expr_buf);
        }
    }
       
    Clr2msFlag();   //清除2ms标志
  }
}

/*********************************************************************************************************
* 函数名称：Proc1SecTask
* 函数功能：1s处理任务 
* 输入参数：void
* 输出参数：void
* 返 回 值：void
* 创建日期：2018年01月01日
* 注    意：
*********************************************************************************************************/
static  void  Proc1SecTask(void)
{ 
  if(Get1SecFlag()) //判断1s标志状态
  {
    Clr1SecFlag();     //清除1s标志
  }    
}


/*********************************************************************************************************
* 函数名称：main
* 函数功能：主函数 
* 输入参数：void
* 输出参数：void
* 返 回 值：int
* 创建日期：2018年01月01日
* 注    意：
*********************************************************************************************************/
int main(void)
{ 
  
  InitHardware();   //初始化硬件相关函数
  InitSoftware();   //初始化软件相关函数
  
  OLED_UI_Refresh();
  printf("简易计算器系统就绪！\r\n");
  printf("操作说明：\r\n");
  printf("1.串口发送算式如3+9；2.KEY1计算；3.KEY2打印结果；4.KEY3清零\r\n");

  while(1)
  {
    FeedIWDG();       // 每轮循环先喂狗
    Proc2msTask();  //2ms处理任务
    Proc1SecTask(); //1s处理任务   
    KeyProcessTask();
  }
}

