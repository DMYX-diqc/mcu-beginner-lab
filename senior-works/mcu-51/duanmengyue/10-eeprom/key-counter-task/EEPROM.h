sfr ISP_DATA=0xE2;  //数据寄存器
sfr ISP_ADDRH=0xE3; //地址寄存器高位
sfr ISP_ADDRL=0xE4; //地址寄存器地位
sfr ISP_CMD=0xE5;   //命令寄存器
sfr ISP_TRIG=0xE6;  //命令触发寄存器
sfr ISP_CONTR=0xE7; //控制寄存器

void IAPSectorErase(unsigned int addr);//擦除指定扇区的函数
void IAPByteWrite(unsigned int addr,unsigned char dat);//字节写入函数
unsigned char IAPByteRead(unsigned int addr);//字节读取函数
