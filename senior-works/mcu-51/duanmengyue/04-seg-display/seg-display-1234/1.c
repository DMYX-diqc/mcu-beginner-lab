#include<reg52.h>

//定义数码管
sbit SegmentG1=P2^3;
sbit SegmentG2=P2^2;
sbit SegmentG3=P2^1;
sbit SegmentG4=P2^0;

//定义数码管显示数字
static unsigned char s_arrNum[]={0x03,0x9f,0x25,0x0d,0x99,0x49,0x41,0x1f,0x01,0x09};

//内部函数声明
static void DelayNms(int nums);
//内部函数实现
static void DelayNms(int nums)
{
  int i,j;
  for(i=0;i<nums;i++)
  {
    for(j=0;j<123;j++){}
  }

}

//主函数
void main()
{
  while(1)
    {
      P0=s_arrNum[1];
      SegmentG1=0;
      DelayNms(5);
      SegmentG1=1;
      
      P0=s_arrNum[2];
      SegmentG2=0;
      DelayNms(5);
      SegmentG2=1;
      
      P0=s_arrNum[3];
      SegmentG3=0;
      DelayNms(5);
      SegmentG3=1;
      
      P0=s_arrNum[4];
      SegmentG4=0;
      DelayNms(5);
      SegmentG4=1;
    }

}
