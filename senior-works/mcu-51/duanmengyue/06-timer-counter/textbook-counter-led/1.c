#include<reg52.h>

sbit LED1=P2^4; //定义LED1

//函数声明
static void InitInterrupt();
static void InitTimer0();
//编写中断配置函数
static void InitInterrupt()
{
  ET0=1;  //打开定时器/计数器的中断允许
  EA=1; //打开总中断允许
}

//编写计数器0配置函数
static void InitTimer0()
{
  TMOD=0x05;
  TH0=0xFF;
  TL0=0xFD;//计数至3后溢出
  TR0=1;//打开计数器0
}

//编写主函数
void main()
{
  InitInterrupt();
  InitTimer0();
  while(1)
    {

    }
}

//编写中断服务函数
void Timer0_Handler() interrupt 1
{
  TH0=0xFF;
  TL0=0xFD;//计数至3后溢出
  LED1=~LED1;
}
