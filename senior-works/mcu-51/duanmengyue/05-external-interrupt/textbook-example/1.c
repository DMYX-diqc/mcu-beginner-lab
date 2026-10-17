#include<reg52.h>

//定义灯泡
sbit LED1=P2^4;
sbit LED2=P2^5;
//定义按键1,2
sbit KEY1=P3^2;
sbit KEY2=P3^3;

//中断配置函数声明
static void InitInterrupt(void);

//编写中断配置函数
static void InitInterrupt()
{
  IT0=1;  //设置外部中断0的触发方式为下降沿触发
  EX0=1;  //打开外部中断0的中断允许
  
  IT1=1;  //设置外部中断1的触发方式为下降沿触发
  EX1=1;  //打开外部中断1的中断允许
  EA=1;   //打开总中断允许
}


//主函数
void main()
{
  InitInterrupt();//中断配置
  while(1)
    {
     
    }
}


//编写外部中断服务函数
void External0() interrupt 0
{
  LED1=~LED1; //翻转LED1的亮灭状态
}


void External1() interrupt 2
{
  LED2=~LED2; //翻转LED2的亮灭状态
}