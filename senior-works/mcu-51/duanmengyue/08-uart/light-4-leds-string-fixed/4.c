#include<reg52.h>
#include<string.h>

sbit LED1=P2^4;
sbit LED2=P2^5;
sbit LED3=P2^6;
sbit LED4=P2^7;

unsigned char Buffer[20];//存储输入的字符串
unsigned char index=0;

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
  unsigned char ch;
  if(RI == 1)
    {
        RI = 0;
        ch = SBUF;
        
   
        if(ch == '#')  // 遇到 # 就结束
        {
            Buffer[index] = '\0';
            index = 0;

            if(strcmp(Buffer, "LED1") == 0)
            {
              LED1 = ~LED1;
            }
            if(strcmp(Buffer, "LED2") == 0)
            {
              LED2 = ~LED2;
            }
            if(strcmp(Buffer, "LED3") == 0)
            {
              LED3 = ~LED3;
            }
            if(strcmp(Buffer, "LED4") == 0)
            {
              LED4 = ~LED4;
            }
        }
        else
        {
            Buffer[index] = ch;
            index++;                     
            if(index >= 20) 
              {
                index = 0;
              }
        }
    }
} 

//编写主函数
void main()
{
  InitUART();
  InitInterrupt();
  LED1=1;
  LED2=1;
  LED3=1;
  LED4=1;
  while(1)
    {
      
    }
}