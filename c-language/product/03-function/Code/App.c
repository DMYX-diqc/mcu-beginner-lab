#include <stdio.h>
#include <stdlib.h> 

//内部函数CalcHour声明
static  short CalcHour(int tick);
//内部函数CalcMin声明
static  short CalcMin(int tick);
//内部函数CalcSec声明
static  short CalcSec(int tick);

//计算小时的内部函数实现
static  short CalcHour(int tick)
{
  short hour;

  hour = tick / 3600;          //tick对3600取模赋值给hour
  
  return(hour);
}

//计算分钟的内部函数实现
static  short CalcMin(int tick)
{
  short min;
  
  min = (tick % 3600) / 60;    //tick对3600取余后再对60取模赋值给min 
  
  return(min);
}

//计算秒的内部函数实现
static  short CalcSec(int tick)
{
  short sec;
  
  sec = (tick % 3600) % 60;    //tick对3600取余后再对60取余赋值给sec
  
  return(sec);
}

//主函数
void  main(void)
{
  int tick = 0;
  
  short hour;
  short min;
  short sec;

  printf("Please input a tick between 0~86399\n");
  scanf_s("%d", &tick);

  hour = CalcHour(tick);
  min  = CalcMin(tick);
  sec  = CalcSec(tick);
  
  printf("Current time: %02d-%02d-%02d\n", hour, min, sec);  

  system("pause");  
}


  

