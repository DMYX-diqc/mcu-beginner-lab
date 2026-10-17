#include <reg52.h>
sbit BEEP=P1^0;
sfr WDT_CONTR=0xE1;//定义看门狗寄存器

//延时函数
static void DelayNms(int nums)
{
  int i,j;
  for(i=0;i<nums;i++)
  {
    for(j=0;j<123;j++){}
  }
}

//编写中断配置函数
static void InitInterrupt()
{
  IT0=1;  //设置外部中断0的触发方式为下降沿触发
  EX0=1;  //打开外部中断0的中断允许
  EA=1;   //打开总中断允许
}

//编写外部中断服务函数
void External0() interrupt 0
{
  DelayNms(2000);//模拟单片机因外部干扰而出现的死机状况
}

//主函数
void main()
{
  unsigned char i;
  
  BEEP=0;
  DelayNms(100);
  BEEP=1;
  
  InitInterrupt();
  WDT_CONTR=0x33;//设置看门狗溢出时间为524.2ms
  
  while(1)
    {
      P2=0xEF;
      for(i=0;i<4;i++)
      {
        DelayNms(500);
        P2=P2<<1;
        WDT_CONTR|=0x10;//喂狗操作
      }
    }
}