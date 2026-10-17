#ifndef _WATCHDOG_H_
#define _WATCHDOG_H_

#include "reg52.h"

//狗窝（看门狗控制寄存器地址）
sfr WDT_CONTR = 0xE1;

//放狗（看门狗初始化）
#define WDT_INIT() (WDT_CONTR = 0x35)  //1.14秒

//喂狗
#define WDT_FEED() (WDT_CONTR |= 0x10)

//拴狗（EEPROM写入前，etc）
#define WDT_DISABLE() (WDT_CONTR = 0x00)

//解狗绳
#define WDT_ENABLE() (WDT_CONTR = 0x35)

#endif