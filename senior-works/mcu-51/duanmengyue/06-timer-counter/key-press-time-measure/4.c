#include<reg52.h>

sbit KEY1=P3^2;
sbit SegmentG1=P2^3;
sbit SegmentG2=P2^2;
sbit SegmentG3=P2^1;
sbit SegmentG4=P2^0;

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

int count=0;
//编写中断服务函数
void Timer0() interrupt 1
{
 
  TH0=0xFC;//重新设置定时器初值
  TL0=0x18;
  
  count++;//每过1ms,count自加
}

//编写定时器0配置函数
static void InitTimer0()
{
  TMOD=0x01;//设置定时器的工作模式1（16位定时器）
  TH0=0xFC;//设置定时1ms后溢出
  TL0=0x18;
  TR0=0;//初始定时器是没有开始计时的
}

//定义数码管显示数字
static unsigned char s_arrNum[]={0x03,0x9f,0x25,0x0d,0x99,0x49,0x41,0x1f,0x01,0x09};

unsigned int qian=0;
unsigned int bai=0; 
unsigned int shi=0;
unsigned int ge=0;  
//编写主函数
void main()
{
  
  InitInterrupt();//定时器中断配置函数[当定时器时间溢出时触发]
  InitTimer0();//定时器0配置函数
  while(1)
    {     
      if(KEY1==0)
        {
          DelayNms(20);
          if(KEY1==0)
            {
              TR0=1;//当开关按下时开始计时
              count=0;//重新计时
              
              while(KEY1==0)
                {
                  //按住按键期间数码管一直显示
                  qian=count/1000;
                  bai=count/100%10;
                  shi=count/10%10;
                  ge=count%10;
                  
                  P0=s_arrNum[qian];SegmentG1=0;DelayNms(5);SegmentG1=1;     
                  P0=s_arrNum[bai];SegmentG2=0;DelayNms(5);SegmentG2=1; 
                  P0=s_arrNum[shi];SegmentG3=0;DelayNms(5);SegmentG3=1;      
                  P0=s_arrNum[ge];SegmentG4=0;DelayNms(5);SegmentG4=1;   
                }
              TR0=0;//按键松开停止计时   
            }
        }
 //==============================松开后保持显示===========================
      qian=count/1000;
      bai=count/100%10;
      shi=count/10%10;
      ge=count%10;
                  
      P0=s_arrNum[qian];SegmentG1=0;DelayNms(5);SegmentG1=1;     
      P0=s_arrNum[bai];SegmentG2=0;DelayNms(5);SegmentG2=1; 
      P0=s_arrNum[shi];SegmentG3=0;DelayNms(5);SegmentG3=1;      
      P0=s_arrNum[ge];SegmentG4=0;DelayNms(5);SegmentG4=1;         
    } 
}