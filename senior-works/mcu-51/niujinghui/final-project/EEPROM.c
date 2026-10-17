#include "reg52.h"
#include "EEPROM.h"

static void IAPTrigger();
static void IAPDisable();

//´¥·¢Æ÷
static void IAPTrigger()
{
  ISP_TRIG = 0x46;
  ISP_TRIG = 0xB9;
}

//¹Ø±ÕIAP
static void IAPDisable()
{
  ISP_CONTR = 0x00;
  ISP_CMD   = 0x00;
  ISP_TRIG  = 0x00;
}

//¶ÁÈ¡
unsigned char IAPByteRead(unsigned int addr)
{
  unsigned char dat;
  ISP_CONTR = 0x81;
  ISP_CMD   = 0x01;
  
  ISP_ADDRL = addr;
  ISP_ADDRH = addr >> 8;

  IAPTrigger();
  dat = ISP_DATA;
  IAPDisable();
  return dat;
}

//²Á³ýÉÈÇø
void IAPSectorErase(unsigned int addr)
{
  ISP_CONTR = 0x81;
  ISP_CMD   = 0x03;

  ISP_ADDRL = addr;
  ISP_ADDRH = addr >> 8;

  IAPTrigger();
  IAPDisable();
}

//Ð´Èë
void IAPByteWrite(unsigned int addr, unsigned char dat)
{
  ISP_CONTR = 0x81;
  ISP_CMD   = 0x02;

  ISP_ADDRL = addr;
  ISP_ADDRH = addr >> 8;
  ISP_DATA = dat;

  IAPTrigger();
	
  IAPDisable();
}
