#include<reg52.h>

sbit KEY1=P3^2;

unsigned char str[]="Hello World";
unsigned char x=0;

//编写串口配置函数
static void InitUART()
{
  SCON=0x50;  //设置串口为工作模式1，并打开接收允许
  TMOD=0x20;  //设置定时器的工作模式为2（8位自动装载定时器）
  PCON=0x80;  //设置波特率加倍
  TL1=0xF3;   //设置定时器1计数初值，波特率为4800值
  TH1=TL1;
  TR1=1;      //打开定时器1
}

//==============编写串口中断配置函数=================//
static void InitInterrupt()
{
  ES=1; //打开串口接收中断允许
  EA=1; //打开总中断允许
}

//=====================发送函数====================//
void SendNext()
{
  if(x<sizeof(str)-1)
    {
      SBUF=str[x];
      x++;
    }
}
//==============编写串口中断服务函数===============//
void UART() interrupt 4
{
  if(TI==1)
    {
      TI=0;
      SendNext();
    }
}

//===================延时函数======================//
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
  InitUART();
  InitInterrupt();
  
  while(1)
    {
      if(KEY1==0)
        {
          Delay(20);
          if(KEY1==0)
            {
             x=0;
             SendNext();//启动第一次发送
             while(KEY1==0);
            }
        }
    }
}