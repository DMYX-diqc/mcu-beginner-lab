#include <stdio.h>
#include <stdlib.h> 

//定义一个时间值结构体，包括三个成员变量，分别是hour，min和sec
typedef struct  
{
  short hour;
  short min;
  short sec;
}StructTimeVal;

//计算小时、分钟和秒值函数声明
static  StructTimeVal CalcTimeVal(int tick);    

//计算小时、分钟和秒，返回值为一个结构体类型的变量
static  StructTimeVal CalcTimeVal(int tick)
{
  StructTimeVal tv;
  
  tv.hour = tick / 3600;            //tick对3600取模赋值给hour
  tv.min  = (tick % 3600) / 60;     //tick对3600取余后再对60取模赋值给min 
  tv.sec  = (tick % 3600) % 60;     //tick对3600取余后再对60取余赋值给sec 
  
  return(tv);
}

//主函数
void main(void)
{
  int tick = 0;  

  StructTimeVal tv;

  printf("Please input a tick between 0~86399\n");
  scanf_s("%d", &tick);

  tv = CalcTimeVal(tick);
  
  printf("Current time: %02d-%02d-%02d\n", tv.hour, tv.min, tv.sec);  

  system("pause");
}