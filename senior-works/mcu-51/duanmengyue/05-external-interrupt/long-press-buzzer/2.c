#include<reg52.h>
//定义按键和蜂鸣器
sbit KEY1=P3^2;
sbit BEEP=P1^0;


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

//中断配置函数声明
static void InitInterrupt(void);

//编写中断配置函数
static void InitInterrupt()
{
  IT0=1;  //设置外部中断0的触发方式为下降沿触发
  EX0=1;  //打开外部中断0的中断允许
  
  EA=1;   //打开总中断允许
}


//主函数
void main()
{
  InitInterrupt();//中断配置
  while(1)
    {
     
    }
}


//编写外部中断服务函数
void External0() interrupt 0
{
  while(1)
  {
    if(0==KEY1)
      {
        Delay(20);
        if(0==KEY1)
          {
            BEEP=0;
            while(0==KEY1);
          }
      }
      BEEP=1;
  }
}