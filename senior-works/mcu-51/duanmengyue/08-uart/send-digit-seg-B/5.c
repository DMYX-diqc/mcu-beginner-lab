#include <reg52.h>

//定义数码管
sbit SegmentG1=P2^3;
sbit SegmentG2=P2^2;
sbit SegmentG3=P2^1;
sbit SegmentG4=P2^0;

 unsigned char arrBuffer[4];

//定义数码管显示数字
static unsigned char s_arrNum[]={0x03,0x9f,0x25,0x0d,0x99,0x49,0x41,0x1f,0x01,0x09};

//延时函数
void DelayMs(unsigned int ms)
{
    unsigned int i, j;
    for(i = 0; i < ms; i++)
        for(j = 0; j < 123; j++); 
}

//编写串口配置函数
static void InitUART()
{
  SCON=0x50;  //设置串口为工作模式1，并打开接收允许
  TMOD=0x20;  //设置定时器的工作模式为2（8位自动装载定时器）
  PCON=0x80;  //设置波特率加倍 
  TL1=0xF3;   //设置定时器1计数初值，波特率为4800
  TH1=TL1;    //打开定时器重新装载，等于计数初值
  TR1=1;      //打开定时器1
}

//编写串口中断配置函数
static void InitInterrupt()
{
  ES=1; //打开串口接收中断允许
  EA=1; //打开总中断允许
}

// 数据处理函数：将整数拆解为千、百、十、个位
void Parse_And_Display(int num)//作用是修改全局变量
{
   
    if(num > 9999)
      {
      num = 9999; // 限制最大值，防止溢出
      }
        
    arrBuffer[0] = num / 1000;          // 千位
    arrBuffer[1] = (num / 100) % 10;    // 百位
    arrBuffer[2] = (num / 10) % 10;     // 十位
    arrBuffer[3] = num % 10;            // 个位
}

// 用于暂存接收到的字符串
    char rx_buffer[6];
    unsigned char rx_index = 0;

//中断服务函数
void UART() interrupt 4
{
  
    int i;
    int val = 0;
    unsigned char temp;

    if(RI == 1)
    {
        RI = 0;
        temp = SBUF;

        // 判断是否接收到结束符 (回车键通常是 \r 0x0D 或 \n 0x0A)
        if(temp == '\r' || temp == '\n')
        {
            rx_buffer[rx_index] = '\0'; // 添加字符串结束符
            val = 0;
            // 将接收到的字符串转换为整数
            // 这里为了不使用复杂的库函数atoi，写一个简单的转换逻辑
            
            for(i=0; i<rx_index; i++) //rx_index=4,i=0,1,2,3
              {
                 // 简单防错：确保是数字字符
                 if(rx_buffer[i]>='0' && rx_buffer[i]<='9') 
                   {
                     val = val * 10 + (rx_buffer[i] - '0');
                   }
              }

            // 拆解并更新显示
            Parse_And_Display(val);//返回arrBuffer数组

            // 重置计数器，准备接收下一组数据
            rx_index = 0;
        }
        else
        {
            // 正常接收数据存入缓冲区
            if(rx_index < 5) // 防止数组越界 (4位数字+1位结束符)
            {
                rx_buffer[rx_index] = temp;
                rx_index++;
            }
            else
            {
                rx_index = 0; // 数据过长重置
            }
        }
    }
}

//主函数
void main()
{
    unsigned char index = 0;
    P0=0xFF;//熄灭段选
    P2=0xFF;//关闭所有位选
  
    InitUART();
    InitInterrupt();
    Parse_And_Display(0);

    while(1)
    {
        P0 = s_arrNum[arrBuffer[index]];
        switch(index )
      {
        case 0:
          SegmentG1=0;
          SegmentG2=1;
          SegmentG3=1;
          SegmentG4=1;
        break;
        case 1:
          SegmentG1=1;
          SegmentG2=0;
          SegmentG3=1;
          SegmentG4=1;
        break;
        case 2:
          SegmentG1=1;
          SegmentG2=1;
          SegmentG3=0;
          SegmentG4=1;
        break;
        default:
          SegmentG1=1;
          SegmentG2=1;
          SegmentG3=1;
          SegmentG4=0;
        break;
      }
  
        DelayMs(2);
      
        // --- 消影 ---
        // 在切换下一位之前，先关闭位选（全部置1）
        // 这能防止中断打断时产生重影
        P2 = 0xFF;
      
      
        index++;
        if(index >= 4)
          { 
            index = 0;
          }
    }
}