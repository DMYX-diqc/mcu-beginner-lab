#include<reg52.h>
#include"EEPROM.h"
//IAP功能即在单片机运行程序的同时，通过软件手段对自身的Flash存储器进行擦除写入的技术
//======================编写内部函数============================//
static void IAPTrigger()
{
  ISP_TRIG=0x46;//对ISP-IAP命令触发寄存器写入触发命令0x46
  ISP_TRIG=0xB9;//对ISP-IAP命令触发寄存器写入触发命令0xB9
}

static void IAPDisable()
{
  ISP_CONTR=0x00;//禁用IAP读写EEPROM
  ISP_CMD=0x00;//无IAP功能
  ISP_TRIG=0x00;//关闭IAP功能
}

//======================编写API函数=============================//
//按字节读取指定扇区数据函数//
unsigned char IAPByteRead(unsigned int addr)
{
  unsigned char dat;  //定义数据缓存变量
  ISP_CONTR=0x81;     //打开IAP功能，允许编程改变Flash，设置Flash操作的等待时间
  ISP_CMD=0x01;       //允许对Data Flash/EEPROM区进行字节读取
  ISP_ADDRL=addr;     //IAP操作地址寄存器低位
  ISP_ADDRH=addr>>8;   //IAP操作地址寄存器高位
  
  IAPTrigger();   //触发IAP功能
  dat=ISP_DATA;   //读出数据
  IAPDisable();   //禁用IAP功能
  return dat;     //将读取的数据作为返回值
}

//擦除指定扇区的函数//
void IAPSectorErase(unsigned int addr)
{
  ISP_CONTR=0x81;     //打开IAP功能，允许编程改变Flash，设置Flash操作的等待时间
  ISP_CMD=0x03;       //允许对Data Flash/EEPROM区进行扇区擦除
  ISP_ADDRL=addr;     //IAP操作地址寄存器低位
  ISP_ADDRH=addr>>8;   //IAP操作地址寄存器高位
  
  IAPTrigger();   //触发IAP功能
  IAPDisable();   //禁用IAP功能
}

//按字节写入指定扇区函数//
void IAPByteWrite(unsigned int addr,unsigned char dat)
{
  ISP_CONTR=0x81;     //打开IAP功能，允许编程改变Flash，设置Flash操作的等待时间
  ISP_CMD=0x02;       //允许对Data Flash/EEPROM区进行字节写入
  ISP_ADDRL=addr;     //IAP操作地址寄存器低位
  ISP_ADDRH=addr>>8;   //IAP操作地址寄存器高位
  ISP_DATA=dat;       //将需要写入的数据放入ISP_DATA
  
  IAPTrigger();   //触发IAP功能
  IAPDisable();   //禁用IAP功能
}