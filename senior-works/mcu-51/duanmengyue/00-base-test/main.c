#include <reg52.h>
sbit LED1=P2^4;
void main()
{
  while(1)
   {
     LED1=0;
   }
}