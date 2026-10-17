/*********************************************************************************************************
* 模块名称：KeyOne.c
* 摘    要：KeyOne模块，进行独立按键初始化，以及按键扫描函数实现
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
#include "KeyOne.h"
#include "stm32f10x_gpio.h"

/*********************************************************************************************************
*                                              宏定义
*********************************************************************************************************/
#define KEY1  (GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1))  //KEY1为读取PC1引脚电平
#define KEY3  (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0))  //KEY3为读取PA0引脚电平
/*********************************************************************************************************
*                                              枚举结构体定义
*********************************************************************************************************/

/*********************************************************************************************************
*                                              内部变量
*********************************************************************************************************/
static u8 s_arrKeyDownLevel[KEY_NAME_MAX]; 
/*********************************************************************************************************
*                                              内部函数声明
*********************************************************************************************************/
static void ConfigKeyOneGPIO(void);
/*********************************************************************************************************
*                                              内部函数实现
*********************************************************************************************************/
//配置按键的GPIO
static void ConfigKeyOneGPIO(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;//GPIO_InitStructure用于存放GPIO的参数
  
  //使能RCC相关时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//使能GPIOA的时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//使能GPIOC的时钟
  
  //配置PC1
  GPIO_InitStructure.GPIO_Pin   =   GPIO_Pin_1;       //设置引脚
  GPIO_InitStructure.GPIO_Mode  =   GPIO_Mode_IPU;    //设置输入类型（上拉输入模式）
  GPIO_Init(GPIOC,&GPIO_InitStructure);               //根据参数初始化GPIO
  
  
  //配置PA0
  GPIO_InitStructure.GPIO_Pin   =   GPIO_Pin_0;       //设置引脚
  GPIO_InitStructure.GPIO_Mode  =   GPIO_Mode_IPU;    //设置输入类型（上拉输入模式）
  GPIO_Init(GPIOA,&GPIO_InitStructure);               //根据参数初始化GPIO
}

/*********************************************************************************************************
*                                              API函数实现
*********************************************************************************************************/
void InitKeyOne(void)
{
  ConfigKeyOneGPIO();//配置按键的GPIO
  
  s_arrKeyDownLevel[KEY_NAME_KEY1]=KEY_DOWN_LEVEL_KEY1;//按键1按下时是低电平
  s_arrKeyDownLevel[KEY_NAME_KEY3]=KEY_DOWN_LEVEL_KEY3;//按键3按下时是低电平

}

void ScanKeyOne(u8 keyName,void(*OnKeyOneUp)(void),void(*OnKeyOneDown)(void))
{
  static u8 s_arrKeyVal[KEY_NAME_MAX];//定义一个数组，用于存放按键的数值
  static u8 s_arrKeyFlag[KEY_NAME_MAX];//定义一个数组，用于存放按键的标志位
  
  s_arrKeyVal[keyName]=s_arrKeyVal[keyName]<<1;//左移一位
  
  switch(keyName)
  {
    case KEY_NAME_KEY1:
      s_arrKeyVal[keyName]=s_arrKeyVal[keyName]| KEY1; //按下/松开时, KEY1为0/1
    break;
    case KEY_NAME_KEY3:
    s_arrKeyVal[keyName]= s_arrKeyVal[keyName]| KEY3; //按下/松开时,KEY3为0/1
    break;
    default:
    break;
  }
  
  //按键标志位的值为TRUE时，判断是否有按键有效按下
  if(s_arrKeyVal[keyName]==s_arrKeyDownLevel[keyName] && s_arrKeyFlag[keyName]==FALSE)
  {
    (*OnKeyOneDown)();              //执行按键按下的响应函数
    s_arrKeyFlag[keyName]= TRUE;   //表示按键处于按下状态，按键标志位的值更改为FALSE
  }
  //按键标志位的值为FALSE时，判断是否有按键有效松开
  else if(s_arrKeyVal[keyName]==(u8)(~s_arrKeyDownLevel[keyName]) && s_arrKeyFlag[keyName]==TRUE)
  {
    (*OnKeyOneUp)();                //执行按键松开的响应函数
    s_arrKeyFlag[keyName]= FALSE;    //表示按键处于松开状态，按键标志位的值更改为TRUE
  }
}