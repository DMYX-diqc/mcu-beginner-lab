#ifndef _SMG_H_
#define _SMG_H_

#include "reg52.h"
#include "public.h"

#define SMG_ADP P0


sbit SEL0 = P2^3;
sbit SEL1 = P2^2;
sbit SEL2 = P2^1;
sbit SEL3 = P2^0;

typedef enum{
	display_time,
	display_date
}DisplayMode;

extern u8 gsmg_code[10];


void SMG_Init(void);

#endif