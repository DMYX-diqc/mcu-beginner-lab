#include <reg52.h>

//定义按键和灯
sbit KEY1=P3^2;
sbit KEY2=P3^3;
sbit KEY3=P3^4;

sbit LED1=P2^4;
sbit LED2=P2^5;
sbit LED3=P2^6;
sbit LED4=P2^7;

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
//样式函数声明
void mode1(void);
void mode2(void);
void mode3(void);



//实现样式函数
//// 模式1：逐个亮并保持 → 逐个熄灭
void mode1()
{
    LED1=0;DelayNms(500);
    LED2=0;DelayNms(500);
    LED3=0;DelayNms(500);
    LED4=0;DelayNms(2000);


    LED4=1;DelayNms(500);
    LED3=1;DelayNms(500);
    LED2=1;DelayNms(500);
    LED1=1;DelayNms(500);  
}

//// 模式2：12亮灭→43亮灭
void mode2()
{
    LED1=0;DelayNms(100);
    LED2=0;DelayNms(100);
    LED1=1;DelayNms(100);
    LED2=1;DelayNms(100);


    LED4=0;DelayNms(100);
    LED3=0;DelayNms(100);
    LED4=1;DelayNms(100);
    LED3=1;DelayNms(100);  
}

//// 模式3：逐个亮 → 逐个熄灭（方向相反）
void mode3()
{
    LED4=0;DelayNms(500);
    LED3=0;DelayNms(500);
    LED2=0;DelayNms(500);
    LED1=0;DelayNms(500);


    LED1=1;DelayNms(500);
    LED2=1;DelayNms(500);
    LED3=1;DelayNms(500);
    LED4=1;DelayNms(500);  
}
//编写主函数
void main()
{
  LED1=1;
  LED2=1;
  LED3=1;
  LED4=1;
  
  while(1)
    {
      if(0==KEY1)
        {
          DelayNms(50);
          if(0==KEY1)
            {
              mode1();
              while(0==KEY1);//等待按键被抬起
            }
        }

        
        
       if(0==KEY2)
        {
          DelayNms(50);
          if(0==KEY2)
            {
              mode2();
              while(0==KEY2);//等待按键被抬起
            }
        }
              
        
        if(0==KEY3)
        {
          DelayNms(50);
          if(0==KEY3)
            {
              mode3();
              while(0==KEY3);//等待按键被抬起
            }
        }
        
    }
}