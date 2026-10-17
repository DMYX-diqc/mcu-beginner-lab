#include<reg52.h>

//定义蜂鸣器
sbit BEEP=P1^0;

//宏定义蜂鸣器开启和关闭
#define BEEP_ON BEEP=0
#define BEEP_OFF BEEP=1

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
  
 //主函数
void main()
{
  BEEP_ON;
  DelayNms(500);
  BEEP_OFF;
  while(1){}
}  