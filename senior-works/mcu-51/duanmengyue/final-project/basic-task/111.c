#include<reg52.h>
#include<stdio.h>
#include <string.h>
#include"EEPROM.h"
//====================定义数码管，按键，小灯泡，蜂鸣器================================//
sbit SegmentG1=P2^3;
sbit SegmentG2=P2^2;
sbit SegmentG3=P2^1;
sbit SegmentG4=P2^0;
sbit KEY1=P3^2;
sbit KEY2=P3^3;
sbit KEY3=P3^4;
sbit KEY4=P3^5;
sbit LED1=P2^4;
sbit LED2=P2^5;
sbit LED3=P2^6;
sbit LED4=P2^7;
sbit BEEP=P1^0;

sfr WDT_CONTR=0xE1;//定义看门狗寄存器

// 选择一个地址来存储时间
#define EEPROM_BASE      0x2000      // 起始地址（一个扇区 512 字节）
#define RECORD_SIZE      9           // 每条记录字节数：1字节的标志位，8字节时间数据
#define RECORD_COUNT     50          // 可存放 50 条记录（50秒），占用 450 字节
#define EEPROM_END       (EEPROM_BASE + RECORD_SIZE * RECORD_COUNT)

// 全局变量：当前写入记录索引（0~RECORD_COUNT-1）
unsigned char current_record_index = 0;

//================================定义时间全局变量====================================//
unsigned char hour=11;
unsigned char min=15;
unsigned char sec=55;
unsigned int year=2026;//char的最大值是255,年份要用int
unsigned char month=4;
unsigned char day=19;
unsigned char week;
bit display_mode=0;//数码管时间日期显示标志变量(0:时间，1:日期)
bit Flag_Send = 0;// 串口发送标志位

//=========================定时器中断计数器，用于积累一秒==============================//
unsigned int TimeCount=0;

//=============================流水灯延迟计数变量=====================================//
unsigned int led_tick=0;//不可以放在中断服务函数中，否则每次进入函数，值都是0

unsigned int alarm_tick=0;//蜂鸣器间接叫计数

//==============================当前扫描的数码管位======================================//
unsigned char current_digit=0;//不可以放在中断服务函数中

//==============================星期字符串数组========================================//
char code *week_str[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

//============================定义数码管显示数字======================================//
unsigned char code s_arrNum[]={0x03,0x9f,0x25,0x0d,0x99,0x49,0x41,0x1f,0x01,0x09};

unsigned char display_buffer[4];//缓冲区显示四位数码管//不可以放在中断服务函数中

// ========================= 新增：串口接收相关变量 ==================================//
bit Flag_Recv_Ready = 0;      // 接收完成标志位
unsigned char Recv_Buffer[7]; // 接收缓冲区，存放 "T235959" 这种指令
unsigned char Recv_Index = 0; // 接收计数

//===============================闹钟相关变量=========================================//
unsigned char alarm_h = 8;    // 闹钟闹钟小时 
unsigned char alarm_m = 30;   // 闹钟闹钟分钟
bit alarm_enable = 1;         // 闹钟使能开关 (1:开启, 0:关闭)
bit alarm_ring_flag = 0;      // 闹铃正在响标志 (1:正在响, 0:未响/已消音)

//=============================执行IAP操作的标志位=======================================//
bit IAP_Flag = 0; //(1:执行, 0:不执行)

//=============================表示按键被按下的标志位=======================================//
unsigned int key2_count = 0;  //按键2延时消抖计数
bit Key2_press_Flag = 0;      //(1:按下, 0:没按下)
bit key1_last_state = 1;      //(0:开启, 1:关闭)

//++++++++++++++++++++++++++++++++++++函数+++++++++++++++++++++++++++++++++++++++++++//

//===========================计算星期函数（蔡勒公式）=================================//
void Calculate_Weekday()
{
  static unsigned char w;
  unsigned int temp_year = year;
  unsigned char temp_month = month;
  //在公式中，1月和2月需要被看作是上一年的13月和14月。因此，m的取值范围是3到14。
  if (temp_month<3)
    {
        temp_month+=12;
        temp_year--;
    }
  //21世纪
  w =((temp_year-2000) + ((temp_year-2000)/4) -35+ (26*(temp_month+1)/10) + day - 1+700)%7;//+700防止取余为负数
  week = w;
}

//=============================往EEPROM中存入时间===============================//
void Save_Time_To_EEPROM()//更新时间
{     
    unsigned int addr;
    // 计算本次写入地址
    addr = EEPROM_BASE + current_record_index * RECORD_SIZE;
  
    //写入标志位
    IAPByteWrite(addr, 0x01);
    //依次写入数据
    // 参数1：地址， 参数2：数据
    IAPByteWrite(addr + 1, hour);
    IAPByteWrite(addr + 2, min);
    IAPByteWrite(addr + 3, sec);
    //因为int占两个字节
    IAPByteWrite(addr + 4, year / 256); //高字节
    IAPByteWrite(addr + 5, year % 256); //低字节
  
    IAPByteWrite(addr + 6, month);
    IAPByteWrite(addr + 7, day);
    IAPByteWrite(addr + 8, week);
  
  // 更新索引（循环）
    current_record_index++;
    if(current_record_index >= RECORD_COUNT)
    {
        // 写满整个区域，擦除整个扇区，重置索引
        IAPSectorErase(EEPROM_BASE);
        current_record_index = 0;
        // 擦除后需要重新写入当前记录（否则数据丢失）
        addr = EEPROM_BASE; 
        IAPByteWrite(addr, 0x01);
        IAPByteWrite(addr + 1, hour);
        IAPByteWrite(addr + 2, min);
        IAPByteWrite(addr + 3, sec);
        IAPByteWrite(addr + 4, year / 256); //高字节
        IAPByteWrite(addr + 5, year % 256); //低字节
  
        IAPByteWrite(addr + 6, month);
        IAPByteWrite(addr + 7, day);
        IAPByteWrite(addr + 8, week);
        
    }
}

//============================从EEPROM读取断电前时间================================//
void Load_Time_From_EEPROM()
{ 
    unsigned char i;
    unsigned int addr;
    unsigned char flag;
    unsigned char found = 0;
  
 for(i = RECORD_COUNT; i > 0; i--)
  {
    addr = EEPROM_BASE + (i-1) * RECORD_SIZE;
    flag = IAPByteRead(addr);
    if(flag == 0x01)
    {
      // 参数：地址
      hour = IAPByteRead(addr + 1);
      min  = IAPByteRead(addr + 2);
      sec  = IAPByteRead(addr + 3);
      // 恢复年份
      year  = IAPByteRead(addr + 4)*256+IAPByteRead(addr + 5); 
      month = IAPByteRead(addr + 6);
      day   = IAPByteRead(addr + 7);
      week  = IAPByteRead(addr + 8);
      current_record_index = i; // 下次写入位置
      found = 1;
      break;
    }
  }
  
  if(!found)
    {
        // 无有效记录，使用默认时间
        hour=11; min=15; sec=55; year=2026; month=4; day=19;
        Calculate_Weekday();
        current_record_index = 0;
    }
}

//===========================更新时间函数（年月日时分秒星期）=================================//
void Update_Clock()
  {
    static unsigned char days_in_month;
    
    sec++;   
    if(sec>=60)
    {
        sec=0;
        min++;
        if(min>=60)
         {
            min=0;
            hour++;
            if(hour>=24)
              {
                hour=0;
                day++;
                // 简单的月份天数判断                  
                if(month == 4 || month == 6 || month == 9 || month == 11)
                  {
                    days_in_month = 30;
                  }
                else if(month == 2) 
                  {
                    // 闰年判断
                    if((year%4==0 && year%100!=0) || (year%400==0)) 
                    {
                      days_in_month = 29;
                    }
                    else days_in_month = 28;
                  }
                else days_in_month = 31;

                if(day > days_in_month)
                {
                    day = 1;
                    month++;
                    if(month > 12)
                    {
                      month = 1;
                      year++;
                    }
                }
                // 日期变动，重新计算星期
                Calculate_Weekday();                                
            }     
         }
      } 
  }

//===============================发送函数=======================================//
void SendByte(unsigned char byte)
{
  SBUF=byte;
  while(!TI);//等待发送完成
  TI=0;//清除标志位
}

//==============================发送字符串函数=====================================//
void SendString(char *str)
{
  while(*str)
    {
      SendByte(*str++);
    }
}

//============================发送数字（自动补零）函数===============================//
void SendNumber(unsigned int num, unsigned char len)
{
    unsigned char i;
    unsigned int divisor = 1;

    // 计算最高位的除数
    for(i=0; i<len-1; i++)
      {
        divisor *= 10;
      }
    for(i=0; i<len; i++)
    {
        SendByte(num / divisor + '0');
        num %= divisor;
        divisor /= 10;
    }
}



//==============================编写串口+中断配置函数================================//
static void InitUART()
{
  SCON=0x50;  //设置串口为工作模式1，并打开接收允许
  TMOD=0x21;  //设置定时器的工作模式为2（8位自动装载定时器）
  PCON=0x80;  //设置波特率加倍
  TL1=0xF3;   //设置定时器1计数初值，波特率为4800值
  TH1=TL1;
  TR1=1;      //打开定时器1
  ES=1; //打开串口接收中断允许
  EA=1; //打开总中断允许
}


//=============================编写串口（定时器1）中断服务函数=============================//
void Timer1() interrupt 4
{
  unsigned char temp;
  if(RI) // 判断是否收到数据
    {
      RI = 0; // 清除接收标志位
      temp = SBUF; // 读取数据
      if(Recv_Index == 0)//Recv_Index是接收计数，检查第一个收到的字符
        {
            if(temp == 'T' || temp == 'D') // 检查是不是 T 或 D
            {
                Recv_Buffer[0] = temp; // 保存头
                Recv_Index ++;        // 索引加1，准备收后面的数字
            }
            // 如果不是T或D，就忽略
        }
        else
        {
            //接收后面的数字
            Recv_Buffer[Recv_Index] = temp;
            Recv_Index++;
            
            //判断是否接收完毕 (1个头 + 6个数字 = 7个字符)
            if(Recv_Index >= 7)
            {
                Flag_Recv_Ready = 1; // 通知主函数处理
                Recv_Index = 0;      // 重置索引，准备下一次接收
            }
        }
     }
}

//=============================编写定时器0+中断配置函数=============================//
static void InitTimer0()
{
  TMOD=0x21;//设置定时器的工作模式1（16位定时器）
  TH0=0xFC;//设置定时1ms后溢出，只要溢出即触发定时器0中断服务函数
  TL0=0x18;
  TR0=1;//打开定时器0
  ET0=1;//打开定时器计数器0的中断允许
}

//=============================编写定时器0中断服务函数================================//
void Timer0() interrupt 1
{
  unsigned char seg_code;//数码管显示数字
   
  LED1=1;LED2=1;LED3=1;LED4=1;  
  
  TH0=0xFC;//重新设置定时器初值
  TL0=0x18;
  
//==============================数码管扫描逻辑===================================// 
  
    P0=0xFF;//数码管消影：关闭所有段选  
    
    seg_code = s_arrNum[display_buffer[current_digit]];
  if(current_digit==1)
    {
      seg_code = seg_code & 0xFE;  //加上小数点
    }
    P0 = seg_code;
    SegmentG1=1;SegmentG2=1;SegmentG3=1;SegmentG4=1;
    switch(current_digit)
    {
      case 0: SegmentG1=0;SegmentG2=1;SegmentG3=1;SegmentG4=1;break;//P2口和LED公用，不能用字节表示
      case 1: SegmentG2=0;SegmentG1=1;SegmentG3=1;SegmentG4=1;break;
      case 2: SegmentG3=0;SegmentG1=1;SegmentG2=1;SegmentG4=1;break;
      case 3: SegmentG4=0;SegmentG1=1;SegmentG2=1;SegmentG3=1;break;
    }
       
    current_digit++;
    if(current_digit >= 4) current_digit = 0;

    
  
  //=============================流水灯逻辑=====================================//
  led_tick++;
  if(led_tick>=200 &&led_tick<=400){LED1=0;}
  else{LED1=1;}
  if(led_tick>=400 &&led_tick<=600){LED2=0;}
  else{LED2=1;}
  if(led_tick>=600 &&led_tick<=800){LED3=0;}
  else{LED3=1;}
  if(led_tick>=800 &&led_tick<=1000){LED4=0;}
  else{LED4=1;}
  if(led_tick==1000){led_tick=0;}

  //============================时间更新逻辑===================================//  
  TimeCount++;
  alarm_tick++;
  if(TimeCount>=1000)//当时间等于一秒才会进行一下操作
    {
      TimeCount=0;
      Flag_Send=1;//通知主函数发送串口
      Update_Clock(); 
      IAP_Flag=1;        
    }
        
      //======数码管显示数字：小时分钟，月日的十位个位计算======//      
        if(display_mode==0)
          {
            display_buffer[0]=hour/10;
            display_buffer[1]=hour%10;
            display_buffer[2]=min/10;
            display_buffer[3]=min%10;
          }
        else
          {
            display_buffer[0]=month/10;
            display_buffer[1]=month%10;
            display_buffer[2]=day/10;
            display_buffer[3]=day%10;
          }
      
   //===============================按键1扫描======================================//
    
    //软件模拟下降沿触发
    // 直接判断：如果上一次是松开(1)，而现在引脚读到的是按下(0)
    if(key1_last_state == 1 && KEY1 == 0)
    {
      display_mode = !display_mode; // 执行功能
    }

    // 更新记忆：把当前的引脚状态存起来，供下一次中断使用
    // 这里直接赋值给 bit 变量，KEY1 会自动被隐式转换为 0 或 1
    key1_last_state = KEY1; 

       
        
    //===============================按键2扫描======================================//      
      if(KEY2==0)
        {
          key2_count++; // 计数器累加，非阻塞式状态机按键扫描 
          if (key2_count > 20)  // 计数超过20ms 
          {
            key2_count = 21; // 锁定计数器，防止溢出
            Key2_press_Flag = 1; // 告诉主程序：按键2已确认按下
          }
        }
      else 
        {
          key2_count = 0; // 如果按键松开，计数器清零
        }  
    
        
         
  //===============================闹钟对比逻辑===================================//
  if(alarm_enable == 1 && hour == alarm_h && min == alarm_m && sec == 0)
      {
          alarm_ring_flag = 1; // 触发闹铃
      }
      
  //===============================蜂鸣器发声逻辑==================================//
  if(alarm_ring_flag == 1)
    {
      if(alarm_tick % 500 < 250) // 响 250ms, 停 250ms
          BEEP = 0; 
      else
          BEEP = 1;
    }
  else
    {
      BEEP = 1;
    }
    
    
}




//=================================主函数编写==================================//
void main()
{

  Load_Time_From_EEPROM();
   
  WDT_CONTR=0x33;//设置看门狗溢出时间为524.2ms
  InitUART();
  InitTimer0();
  Calculate_Weekday();
        
  while(1)
   {
     WDT_CONTR|=0x10;//喂狗操作
     
     //在主函数中执行IAP操作（不能在中断服务函数中执行）
     if(IAP_Flag==1)
       {
         EA = 0;
         Save_Time_To_EEPROM(); 
         IAP_Flag=0;
         EA = 1;
       }
       
        
     //执行按键2的具体功能 
     if(Key2_press_Flag==1)
       {
         Key2_press_Flag = 0; // 清除标志 
         if(alarm_ring_flag == 1)
              {
                  alarm_ring_flag = 0; // 关闭闹铃标志，蜂鸣器停止
                  BEEP = 1;                 // 立即关闭
              }
       }  
       
   //串口发送很慢，如果放在中断服务函数，这段时间CPU会卡在中断里，影响数码管扫描和时间计数
     //================================串口打印逻辑====================================//
      //格式:2022-07-13 10:24:23 wed//
      if(Flag_Send==1)
        {
          Flag_Send=0;
          SendNumber(year, 4);
          SendString("-");
          SendNumber(month, 2);
          SendString("-");
          SendNumber(day, 2);
          SendString(" ");
          SendNumber(hour, 2);
          SendString(":");
          SendNumber(min, 2);
          SendString(":");
          SendNumber(sec, 2);
          SendString(" ");
          SendString(week_str[week]);
          SendString("\r\n"); // 换行 
        }
      
    //=================================串口设置时间逻辑==================================//
      if(Flag_Recv_Ready == 1)
        {
          Flag_Recv_Ready = 0; // 清除标志
          
          // 解析时间指令 "T235959"
          if(Recv_Buffer[0] == 'T')
            {           //将字符类型的数字转换成它对应的实际数值
              hour = (Recv_Buffer[1] - '0') * 10 + (Recv_Buffer[2] - '0');
              min  = (Recv_Buffer[3] - '0') * 10 + (Recv_Buffer[4] - '0');
              sec  = (Recv_Buffer[5] - '0') * 10 + (Recv_Buffer[6] - '0');
              IAP_Flag=1;
              SendString("Time Set OK!\r\n");
            }
          // 解析日期指令 "D260416"
          if(Recv_Buffer[0] == 'D')
            {
              year = 2000 + (Recv_Buffer[1] - '0') * 10 + (Recv_Buffer[2] - '0');
              month= (Recv_Buffer[3] - '0') * 10 + (Recv_Buffer[4] - '0');
              day  = (Recv_Buffer[5] - '0') * 10 + (Recv_Buffer[6] - '0');
              Calculate_Weekday();
              IAP_Flag=1;
              SendString("Date Set OK!\r\n");
            }            
        } 
   }
}