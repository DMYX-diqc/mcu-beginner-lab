#include<reg52.h>

//编写串口配置函数
static void InitUART()
{
  SCON=0x50;  //设置串口为工作模式1，并打开接收允许
  TMOD=0x20;  //设置定时器的工作模式为2（8位自动装载定时器）
  PCON=0x80;  //设置波特率加倍 
  TL1=0xF3;   //设置定时器1计数初值，波特率为4800
  TH1=TL1;    //打开定时器重新装载，等于计数初值
  TR1=1;      //打开定时器1
}

//编写串口中断配置函数
static void InitInterrupt()
{
  ES=1; //打开串口接收中断允许
  EA=1; //打开总中断允许
}

//编写串口中断服务函数
void UART() interrupt 4
{
  static unsigned char Buffer;//定义缓冲变量
  Buffer=SBUF;//将数据从寄存器中取出
  RI=0;//清除接收中断请求
  
  SBUF=Buffer;//写入发送数据
  while(!TI)
    {

    }//等待数据发送完成
  TI=0;
}

//编写主函数
void main()
{
  InitUART();
  InitInterrupt();
  while(1)
    {
    
    }
}