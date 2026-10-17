#include <reg52.h>

sbit LED1=P2^4;
sbit LED2=P2^5;
sbit LED3=P2^6;
sbit LED4=P2^7;

sbit KEY2=P3^3;

//声明
static void DelayNms(unsigned int nms);
static void mode1();
static void mode2();

//定义
static void DelayNms(unsigned int nms)
{
    unsigned int i,j;
    for(i = 0; i < nms; i++)
      {
        for(j = 0; j < 123; j++){}
      }
}

//// 模式1：逐个亮并保持 → 逐个熄灭
static void mode1()
{
    LED1=0;DelayNms(100);
    LED2=0;DelayNms(100);
    LED3=0;DelayNms(100);
    LED4=0;DelayNms(2000);


    LED4=1;DelayNms(100);
    LED3=1;DelayNms(100);
    LED2=1;DelayNms(100);
    LED1=1;DelayNms(100);  
}

//// 模式2：13亮，24亮 → 13灭，24灭
static void mode2()
{
    LED1=0;DelayNms(5);
    LED3=0;DelayNms(400);
    LED2=0;DelayNms(5);
    LED4=0;DelayNms(400);


    LED1=1;DelayNms(5);
    LED3=1;DelayNms(400);
    LED2=1;DelayNms(5);
    LED4=1;DelayNms(400);  
}

//中断配置函数声明
static void InitInterrupt(void);

//编写中断配置函数
static void InitInterrupt()
{
  IT1=1;  //设置外部中断0的触发方式为下降沿触发
  EX1=1;  //打开外部中断0的中断允许
  
  EA=1;   //打开总中断允许
}


//主函数
unsigned int mode;
void main()
{
  InitInterrupt();//中断配置
  while(1)
    {
     if(mode==1)
       {
        mode1();
       }
     if(mode==2)
      {
       mode2();
      }       
    }
}


//编写外部中断服务函数
void External0() interrupt 2
{
  unsigned int count=0;
  if(0==KEY2)
    {
      DelayNms(20);
      if(0==KEY2)
        {
          while(0==KEY2)
            {
              DelayNms(10);
              count++;
              if(count>=50)
                {
                  mode=2;
                  break;
                }
                mode=1;
            }
        }
    }
  
}