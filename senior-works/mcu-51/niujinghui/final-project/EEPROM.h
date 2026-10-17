#ifndef _EEPROM_H_
#define _EEPROM_H_

sfr ISP_DATA  = 0XE2;
sfr ISP_ADDRH = 0XE3;
sfr ISP_ADDRL = 0XE4;
sfr ISP_CMD   = 0XE5;
sfr ISP_TRIG  = 0XE6;
sfr ISP_CONTR = 0XE7;

void IAPSectorErase(unsigned int addr);
void IAPByteWrite(unsigned int addr, unsigned char dat);
unsigned char IAPByteRead(unsigned int addr);

#endif