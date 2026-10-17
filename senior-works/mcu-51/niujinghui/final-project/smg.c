#include "smg.h"
#include "public.h"


unsigned char gsmg_code[10] = {0x03 , 0x9f , 0x25 , 0x0d , 0x99 , 0x49 , 0x41 , 0x1f , 0x01 , 0x09};

static DisplayMode current_mode=display_time;

void SMG_Init(void) {
    SMG_ADP = 0x00;
    SEL0 = 1;
    SEL1 = 1;
    SEL2 = 1;
    SEL3 = 1;
    current_mode = display_time;
}