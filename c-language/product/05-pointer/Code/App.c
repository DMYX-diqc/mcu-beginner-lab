#include <stdio.h>
#include <stdlib.h> 

//计算小时、分钟和秒值函数声明
static  unsigned char CalcTimeVal(int tick, short* pTimeVal);

//计算小时、分钟和秒值函数实现，返回值表示是否计算成功，0-失败，1-成功
static  unsigned char CalcTimeVal(int tick, short* pTimeVal)
{
  unsigned char valid = 0;    //初始值为0，即表示无效/失败
  
  if(tick >= 0 &&  tick <= 86399)
  {
    valid = 1;                //表示有效/成功

    *(pTimeVal+2) = tick / 3600;            //tick对3600取模赋值给hour
    *(pTimeVal+1) = (tick % 3600) / 60;     //tick对3600取余后再对60取模赋值给min 
    *(pTimeVal+0) = (tick % 3600) % 60;     //tick对3600取余后再对60取余赋值给sec
  }
  
  return(valid);
}

//主函数
void main(void)
{
  int tick = 0;  

  short arrTimeVal[3]; //[2]-hour，[1]-min，[0]-sec

  unsigned char calcRightFlag = 0; //计算正确与否标志，1-正确，0-不正确

  printf("Please input a tick between 0~86399\n");
  scanf_s("%d", &tick);
  
  calcRightFlag = CalcTimeVal(tick, arrTimeVal);

  if(1 == calcRightFlag)
  {
    printf("Current time: %02d-%02d-%02d\n", arrTimeVal[2], arrTimeVal[1], arrTimeVal[0]);  
  }
  else
  {
    printf("Invalid tick value");
  }

  system("pause");
}



    


    



  

