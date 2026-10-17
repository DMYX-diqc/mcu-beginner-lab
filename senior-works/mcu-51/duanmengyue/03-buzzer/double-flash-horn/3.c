#include<reg52.h>

//定义按键和灯
sbit KEY1=P3^2;
sbit KEY2=P3^3;
sbit KEY3=P3^4;

sbit LED1=P2^4;
sbit LED4=P2^7;

//定义蜂鸣器
sbit BEEP=P1^0;

//宏定义蜂鸣器开启和关闭
#define BEEP_ON BEEP=0
#define BEEP_OFF BEEP=1

//状态变量
bit LEFT_ON =0;
bit RIGHT_ON=0;
bit DOUBLE_ON=0;


//内部函数声明
static void DelayNms(int nums);
//内部函数实现
static void DelayNms(int nums)
{
  int i,j;
  for(i=0;i<nums;i++)
  {
    for(j=0;j<123;j++){}
  }

}

//按键扫描//
void key_scan()
{
//KEY1左转向
  if(0==KEY1)
    {
      DelayNms(10);
      if(0==KEY1)
        {
          LEFT_ON=~LEFT_ON;
          RIGHT_ON=0;
          DOUBLE_ON=0;
          LED1=1;
          LED4=1;
          BEEP=1;
          while(0==KEY1);
        }
    }
    
//KEY2右转向
  if(0==KEY2)
    {
      DelayNms(10);
      if(0==KEY2)
        {
          RIGHT_ON=~RIGHT_ON;
          DOUBLE_ON=0;
          LEFT_ON=0;
          LED1=1;
          LED4=1;
          BEEP=1;
          while(0==KEY1);
        }
    }

//KEY3双闪
  if(0==KEY3)
    {
      DelayNms(10);
      if(0==KEY3)
        {
          DOUBLE_ON=~DOUBLE_ON;
          LEFT_ON=1;
          RIGHT_ON=1;
          LED1=1;
          LED4=1;
          BEEP=1;
          while(0==KEY3);
        }
    }    
}
  
  //主函数
  void main()
{
  //初始化灯和蜂鸣器
  LED1=1;LED4=1;BEEP=1;
  while(1)
    {
      key_scan();
      
         //灯控制
      if(DOUBLE_ON)
        {
          LED1=0;LED4=0;BEEP=0;
          DelayNms(500);
          key_scan();
          
          LED1=1;LED4=1;BEEP=1;
          DelayNms(500);
          key_scan();
        }
      
      else if(LEFT_ON)
        {
          LED1=0;LED4=1;BEEP=0;
          DelayNms(500);
          key_scan();
          
          LED1=1;LED4=1;BEEP=1;
          DelayNms(500);
          key_scan();
        }
        
      else if(RIGHT_ON)
        {
          LED1=1;LED4=0;BEEP=0;
          DelayNms(500);
          key_scan();
          
          LED1=1;LED4=1;BEEP=1;
          DelayNms(500);
          key_scan(); 
        }

      else
        {
         LED1=1;LED4=1;BEEP=1;
        }        
    }
 }