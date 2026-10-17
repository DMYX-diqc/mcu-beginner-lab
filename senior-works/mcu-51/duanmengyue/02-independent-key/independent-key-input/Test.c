#include<reg52.h>
//定义按键和灯
sbit KEY1 = P3^2;
sbit KEY2 = P3^3;
sbit KEY3 = P3^4;

sbit LED1 = P2^4;
sbit LED2 = P2^5;
sbit LED3 = P2^6;

//内部函数声明
static void Delay(int nums);
//内部函数实现
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
  LED1=0;
  LED2=0;
  LED3=0;
  
  while(1)
    {
      if(0==KEY1)//第一次检验KEY1按键是否被按下
        {
          Delay(50);  //等待约50ms后再次检测按键是否被按下，消除按键抖动带来的影响
          if(0==KEY1)
            {
              LED1=~LED1;//对LED状态取反，改变亮暗状态
              while(0==KEY1);//等待按键被弹起
            }
        }


      if(0==KEY2)//第一次检验KEY2按键是否被按下
        {
          Delay(50);
          if(0==KEY2)
            {
              LED2=~LED2;
              while(0==KEY2);
            }
        }



      if(0==KEY3)//第一次检验KEY3按键是否被按下
        {
          Delay(50);
          if(0==KEY3)
            {
              LED3=~LED3;
              while(0==KEY3);
            }
        }
    }
}