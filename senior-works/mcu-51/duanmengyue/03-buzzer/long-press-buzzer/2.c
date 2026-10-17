#include<reg52.h>
//定义按键和蜂鸣器
sbit KEY1=P3^2;
sbit BEEP=P1^0;

//宏定义蜂鸣器开启和关闭
#define BEEP_ON BEEP=0
#define BEEP_OFF BEEP=1

//内部函数声明
static void Delay(int nums);
//内部函数实现
static void Delay(int nums)
{
  int i,j;
  for(i=0;i<nums;i++)
  {
    for(j=0;j<123;j++){}
  }

}


//编写主函数
void main()
{
  while(1)
    {
      if(0==KEY1)
        {
          Delay(50);
          if(0==KEY1)
            {
              BEEP_ON;
              while(0==KEY1);//等待按键被抬起
            }
        }
        BEEP_OFF;
    }
}