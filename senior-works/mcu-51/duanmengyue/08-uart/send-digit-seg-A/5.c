#include<reg52.h>

//定义数码管
sbit SegmentG1=P2^3;
sbit SegmentG2=P2^2;
sbit SegmentG3=P2^1;
sbit SegmentG4=P2^0;

//定义数码管显示数字
static unsigned char s_arrNum[]={0x03,0x9f,0x25,0x0d,0x99,0x49,0x41,0x1f,0x01,0x09};

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

unsigned char index=0;
unsigned char arrBuffer[4];

//编写串口中断服务函数
void UART() interrupt 4
{
  static unsigned char num;
  if(RI==1)
    {
      RI=0;//清除
      num=SBUF;//提取一个数字
    }
  if(num>='0' && num<='9')
    {
      arrBuffer[index]=num-'0';     //index=0,1,2,3
      index++;
    }

  if(index>3)
    {
      index=0;
    }
}


//编写主函数
void main()
{
  unsigned char scan_index=0;
  InitUART();
  InitInterrupt();
  
  while(1)
    {
      P0=0xFF;//熄灭段选
      P2=0xFF;//关闭所有位选
      
      P0=s_arrNum[arrBuffer[scan_index]];//段选，数码管显示什么
      
//      P2 = ~(0x01 << scan_index);//位选,依次打开数码管1234
      if(scan_index == 0)
      {
          SegmentG1 = 0; // 第1个数码管亮
          SegmentG2 = 1;
          SegmentG3 = 1;
          SegmentG4 = 1;
      }
      else if(scan_index == 1)
      {
          SegmentG1 = 1;
          SegmentG2 = 0; // 第2个数码管亮
          SegmentG3 = 1;
          SegmentG4 = 1;
      }
      else if(scan_index == 2)
      {
          SegmentG1 = 1;
          SegmentG2 = 1;
          SegmentG3 = 0; // 第3个数码管亮
          SegmentG4 = 1;
      }
      else // scan_index == 3
      {
          SegmentG1 = 1;
          SegmentG2 = 1;
          SegmentG3 = 1;
          SegmentG4 = 0; // 第4个数码管亮
      }
      
      scan_index++;   //scan_index=0,1,2,3
      if(scan_index>3)
        {
          scan_index=0;
        }
      
    }
}