#include<reg52.h>

//定义数码管
sbit SegmentG1=P2^3;
sbit SegmentG2=P2^2;

//定义按键
sbit KEY1=P3^2;

//定义数码管显示数字
static unsigned char s_arrNum[]={0x03,0x9f,0x25,0x0d,0x99,0x49,0x41,0x1f,0x01,0x09};

//内部函数声明
static void DelayNms(int nums);
static void InitInterrupt(void);
//内部函数实现
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

//主函数
void main()
{
  InitInterrupt();
  while(1)
    {

    }
}

//编写外部中断服务函数
void External0() interrupt 0
{
  static unsigned int count=0;
  static unsigned int tens=0;
  static unsigned int units=0;
   while(1)
    {
      if(0==KEY1)
        {
          DelayNms(20);
          if(0==KEY1)
            {
              count++;
              while(0==KEY1);
            }
         }
         
      tens=count/10;
      units=count%10;
         
      P0=s_arrNum[tens];
      SegmentG1=0;
      DelayNms(5);
      SegmentG1=1;
      
      P0=s_arrNum[units];
      SegmentG2=0;
      DelayNms(5);
      SegmentG2=1; 

      if(count>99)
        {
          count=0;
        }
    }
}