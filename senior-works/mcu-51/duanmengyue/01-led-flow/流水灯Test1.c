#include <reg52.h>

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

//// 模式1：逐个亮并保持 → 逐个熄灭
void mode1()
{
    LED1=0;DelayNms(100);
    LED2=0;DelayNms(100);
    LED3=0;DelayNms(100);
    LED4=0;DelayNms(2000);


    LED4=1;DelayNms(100);
    LED3=1;DelayNms(100);
    LED2=1;DelayNms(100);
    LED1=1;DelayNms(100);  
}

void main()
{
    

    while(1)
    {
        mode1();
    }
}

