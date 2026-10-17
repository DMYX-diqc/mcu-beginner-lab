/*********************************************************************************************************
* 模块名称：ProcKeyOne.c
* 摘    要：ProcKeyOne模块，进行独立按键处理模块初始化，以及独立按键处理函数实现
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
#include "ProcKeyOne.h"
#include "stm32f10x_gpio.h"
#include "UART1.h"
#include "Main.h"
#include "Calculator.h"
#include "SysTick.h"
#include "OLED.h"
#include "EXTI.h"
#define KEY1  (GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1))  //KEY1为读取PC1引脚电平
#define KEY2  (GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_2))  //KEY1为读取PC2引脚电平
#define KEY3  (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0))  //KEY3为读取PA0引脚电平
/*********************************************************************************************************
*                                              内部变量
*********************************************************************************************************/
uint8_t key_int_flag = 0;
uint8_t trigger_key_id = 0;
uint32_t key_press_tick = 0;

/*********************************************************************************************************
*                                              API函数实现
*********************************************************************************************************/

void InitProcKeyOne(void)
{
  
}
//===============按键1==============//
void ProcKeyDownKey1(void)
{
  printf("开始计算：");
  if(flag_has_expr == 0)
    {
        printf("错误：未收到有效运算式！\r\n");
        return;
    }
    ParseAndCalcExpr(); //调用计算函数
}

void ProcKeyUpKey1(void)
{
  
}

//===============按键2==============//
void ProcKeyDownKey2(void)
{
  printf("发送结果：");
   // 未输入表达式 / 已清零
    if(flag_has_expr == 0 || flag_calc_done == 0)
    {
        printf("请输入运算式\r\n");
        return;
    }
    // 打印算式与十进制结果
    printf("算式 %s = %.2f\r\n", expr_buf, calc_result);
}

void ProcKeyUpKey2(void)
{
  
}

//===============按键3==============//
void ProcKeyDownKey3(void)
{
  printf("清零计算器：");
  InitCalculator();
}

void ProcKeyUpKey3(void)
{
  
}

/*********************************************************************************************************
* 函数名称：KeyProcessTask
* 函数功能：主循环周期调用，处理EXTI中断触发的按键，区分长短按
* 入参：sys_tick_ms 系统全局毫秒计时
*********************************************************************************************************/
void KeyProcessTask(void) 
{
    uint32_t press_dur;
   
    uint8_t key_pin_state;

    // 无按键中断触发，直接退出
    if(key_int_flag == 0)
        return;

    // -------------------------- 必须等待松手，否则Up函数提前执行 --------------------------
    while(1)
    {
        // 根据触发的按键ID读取对应引脚电平
        switch(trigger_key_id)
        {
            case 1: key_pin_state = KEY1; break;
            case 2: key_pin_state = KEY2; break;
            case 3: key_pin_state = KEY3; break;
            default: key_pin_state = 1; break;
        }
        // 上拉输入：松开=高电平1，退出等待循环
        if(key_pin_state == 1)
            break;     
    }
    // --------------------------------------------------------------------------------------

    // 松手后，计算完整按压时长
    press_dur = systick - key_press_tick;
    key_int_flag = 0; // 清除中断标记

    // 短按逻辑：松手后执行按下回调
    if(press_dur < KEY_LONG_MS)
    {
        switch(trigger_key_id)
        {
            case 1: ProcKeyDownKey1(); break;
            case 2: ProcKeyDownKey2(); break;
            case 3: ProcKeyDownKey3(); break;
        }
    }
    else
    {
        // 仅KEY3长按切换页面
        if(trigger_key_id == 3)
        {
            oled_page = !oled_page;
            printf("切换页面\r\n");
        }
    }

    // 只有松手完成，才执行松开回调函数
    switch(trigger_key_id)
    {
        case 1: ProcKeyUpKey1(); break;
        case 2: ProcKeyUpKey2(); break;
        case 3: ProcKeyUpKey3(); break;
    }

    OLED_UI_Refresh();
}
