#include <stdio.h>
#include <stdlib.h> 

void  main(void)
{
  int tick = 0; //0~86399
  
  short hour; //小时值
  short min;  //分钟值
  short sec;  //秒值
  
  printf("Please input a tick between 0~86399\n");
  scanf_s("%d", &tick);
  
  hour =  tick / 3600;          //tick对3600取模赋值给hour  
  min  = (tick % 3600) / 60;    //tick对3600取余后再对60取模赋值给min  
  sec  = (tick % 3600) % 60;    //tick对3600取余后再对60取模赋值给sec
  
  //打印转换之后的结果
  printf("Current time: %02d-%02d-%02d\n", hour, min, sec);  
  system("pause");
}
