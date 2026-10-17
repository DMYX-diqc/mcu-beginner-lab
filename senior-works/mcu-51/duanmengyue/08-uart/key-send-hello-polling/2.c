#include<reg52.h>

sbit KEY1=P3^2;

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

//===============发送单个字符=================//
void Sendchar(unsigned char ch)
{
  SBUF=ch;
  while(!TI);//等待当前字节发送完成
  TI=0;
}

//==============发送字符串====================//
void SendString(unsigned char *str)
{
  while(*str!='\0')
    {
     Sendchar(*str);
     str++;//str是指针，指向这串字符开头的地址
    }
}

//=====================延时函数==============//
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
  while(1)
    {
      if(KEY1==0)
        {
          Delay(20);
          if(KEY1==0)
            {
             SendString("Hello World");
             while(KEY1==0);
            }
        }
    }
}