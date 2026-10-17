#include <stdio.h>
#include <stdlib.h> 

void  main(void)
{
  int tick = 0; //0~86399
  
  short arrTimeVal[3]; //存放小时值、分钟值、秒值
  
  printf("Please input a tick between 0~86399\n");
  scanf_s("%d", &tick);
  
  arrTimeVal[2] =  tick / 3600;           //tick对3600取模赋值给hour  
  arrTimeVal[1] = (tick % 3600) / 60;     //tick对3600取余后再对60取模赋值给min  
  arrTimeVal[0] = (tick % 3600) % 60;     //tick对3600取余后再对60取模赋值给sec
  
  printf("Current time: %02d-%02d-%02d\n", arrTimeVal[2], arrTimeVal[1], arrTimeVal[0]); 
  
  system("pause");
}
