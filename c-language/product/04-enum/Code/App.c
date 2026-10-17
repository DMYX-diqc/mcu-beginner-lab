#include <stdio.h>
#include <stdlib.h> 

//定义枚举
typedef enum  
{
  TIME_VAL_HOUR = 0,
  TIME_VAL_MIN,
  TIME_VAL_SEC,
  TIME_VAL_MAX
}EnumTimeVal;

//计算小时、分钟和秒值函数声明
static  short CalcTimeVal(int tick, unsigned char type);


//计算小时、分钟和秒值函数实现
static  short CalcTimeVal(int tick, unsigned char type)
{
  short timeVal;
  
  switch(type)
  {
  case TIME_VAL_HOUR:
    timeVal = tick / 3600;            //tick对3600取模赋值给hour
    break;
  case TIME_VAL_MIN:
    timeVal = (tick % 3600) / 60;     //tick对3600取余后再对60取模赋值给min 
    break;
  case TIME_VAL_SEC:
    timeVal = (tick % 3600) % 60;     //tick对3600取余后再对60取余赋值给sec
    break;
  default:
    break;  
  }
  
  return(timeVal);
}

//主函数
void main(void)
{
  int tick = 0;
  
  short hour;
  short min;
  short sec; 
  
  printf("Please input a tick between 0~86399\n");
  scanf_s("%d", &tick);
  
  hour = CalcTimeVal(tick, TIME_VAL_HOUR);
  min  = CalcTimeVal(tick, TIME_VAL_MIN);
  sec  = CalcTimeVal(tick, TIME_VAL_SEC);
  
  printf("Current time: %02d-%02d-%02d\n", hour, min, sec);  

  system("pause");  
}
 

    



  

