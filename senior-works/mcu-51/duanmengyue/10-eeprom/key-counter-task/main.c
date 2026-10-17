#include<reg52.h>
#include"EEPROM.h"
//定义数码管
sbit SegmentG1=P2^3;
sbit SegmentG2=P2^2;

//定义按键
sbit KEY1=P3^2;

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
  static unsigned int count=0;
  static unsigned int tens=0;
  static unsigned int units=0;
  //开机恢复数据：从 Flash 读取数值赋给 count
  unsigned char temp = IAPByteRead(0x2000);
  if(temp != 0xFF) 
    {
      count = temp; 
    }
  
  while(1)
    {
      if(0==KEY1)
        {
          DelayNms(20);
          if(0==KEY1)
            {
              count++;
              if(count > 99) count = 0; // 限制范围
              while(0==KEY1);
              IAPSectorErase(0x2000);//擦除起始地址为0x2000的扇区
              IAPByteWrite(0x2000,count);//对起始地址为0x2000的扇区写入P2寄存器的值
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
    }
}