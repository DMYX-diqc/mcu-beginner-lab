#include<reg52.h>

sbit BEEP=P1^0;
sbit KEY1=P3^2;
sbit SegmentG1=P2^3;
sbit SegmentG2=P2^2;

//编写延迟函数
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

//定义数码管显示数字
static unsigned char s_arrNum[]={0x03,0x9f,0x25,0x0d,0x99,0x49,0x41,0x1f,0x01,0x09};

int sec=10;
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
      sec=sec-1;
    }
}

//编写主函数
void main()
{
  BEEP=1;
  InitInterrupt();//配置中断
  InitTimer0();//配置定时器0
  while(1)
    {
      static unsigned int tens=0;
      static unsigned int units=0;
      
      tens=sec/10;
      units=sec%10;
      
      P0=s_arrNum[tens];
      SegmentG1=0;
      DelayNms(5);
      SegmentG1=1;
      
      P0=s_arrNum[units];
      SegmentG2=0;
      DelayNms(5);
      SegmentG2=1; 
      
      if(tens==0 && units==0)
        {
          BEEP=0;
        }
      if(KEY1==0)
        {
          DelayNms(20);
          if(KEY1==0)
            {
              TH0=0xFC;//重新设置定时器初值
              TL0=0x18;
              sec=10;
              tens=0;
              units=0;
              BEEP=1;
              while(KEY1 == 0);
            }
        }
    }
}