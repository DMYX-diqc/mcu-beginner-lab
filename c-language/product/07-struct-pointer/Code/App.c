#include <stdio.h>
#include <stdlib.h> 

//声明一个时间值结构体，包括三个成员变量，分别是hour，min和sec
typedef struct  
{
  short hour;
  short min;
  short sec;
}StructTimeVal;

//计算小时、分钟和秒值函数声明
static  unsigned char CalcTimeVal(int tick, StructTimeVal* pTV);

//计算小时、分钟和秒值函数实现，返回值表示是否计算成功，0-失败，1-成功
static  unsigned char CalcTimeVal(int tick, StructTimeVal* pTV)
{
  unsigned char valid = 0;  

  if(tick >= 0 &&  tick <= 86399)
  {
    valid = 1;
    pTV->hour = tick / 3600;            //tick对3600取模赋值给hour
    pTV->min  = (tick % 3600) / 60;     //tick对3600取余后再对60取模赋值给min 
    pTV->sec  = (tick % 3600) % 60;     //tick对3600取余后再对60取模赋值给sec 
  }
  
  return(valid);
}

//主函数
void main(void)
{
  int tick = 0;

  unsigned char valid;
  
  StructTimeVal tv;

  printf("Please input a tick between 0~86399\n");
  scanf_s("%d", &tick);  

  valid = CalcTimeVal(tick, &tv);
  
  if(valid > 0)
  {
    printf("Current time: %02d-%02d-%02d\n", tv.hour, tv.min, tv.sec);  
  }
  else
  {
    printf("Invalid tick value!!!\n");
  }

  system("pause");
}



    


    



  

