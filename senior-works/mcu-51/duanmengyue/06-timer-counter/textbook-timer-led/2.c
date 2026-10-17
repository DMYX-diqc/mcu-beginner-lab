#include<reg52.h>

sbit LED1=P2^4;

//编写中断配置函数
static void InitInterrupt()
{
  ET0=1;//打开定时器计数器0的中断允许
  EA=1;//打开总中断允许
}

//编写定时器0配置函数
static void InitTimer0()
{
  TMOD=0x01;//设置定时器的工作模式1（16位定时器）
  TH0=0xFC;//设置定时1ms后溢出
  TL0=0x18;
  TR0=1;//打开定时器0
}

//编写主函数
void main()
{
  InitInterrupt();//配置中断
  InitTimer0();//配置定时器0
  while(1){}
}

//编写中断服务函数
void Timer0() interrupt 1
{
  static int count=0;
  TH0=0xFC;//重新设置定时器初值
  TL0=0x18;
  
  count++;
  if(count>=1000)
    {
      count=0;
      LED1=~LED1;
    }
}