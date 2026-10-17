#include "reg52.h"
#include "public.h"
#include "smg.h"
#include "EEPROM.h"
#include <string.h>
#include <intrins.h>
#include "watchdog.h"

#define UART_BUF_LEN  32
#define PWM_PERIOD 10        					//PWM周期（1-255，大=平滑）
#define FADE_STEP 1          					//每次亮度变化步长（1-20，大=快）
#define FADE_INTERVAL 150    				  //亮度调整间隔（ms）

#define BEEP_TONE1_FREQ 2000  				//低音
#define BEEP_TONE2_FREQ 3000  				//中音
#define BEEP_TONE3_FREQ 4000  				//高音
#define BEEP_TONE_CHANGE_MS 500
#define BEEP_TOTAL_DURATION 1500

int keyflag = 0;
int weekflag = 0;
int year = 2026;
int month = 9;
int day = 11;
int hour = 8;
int min = 1;
int sec = 0;
int alarm_hour = 0;
int alarm_min = 0;
int alarm_sec = 5;

static const char* weekdays[] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat","Sun"};
static const u8 max_days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

sbit LED1 = P2^4;
sbit LED2 = P2^5;
sbit LED3 = P2^6;
sbit LED4 = P2^7;

sbit BEEP = P1^0;

sbit KEY1 = P3^2;			//用于关闭蜂鸣器闹铃
sbit KEY2 = P3^3;			//用于设置闹铃时间
sbit KEY3 = P3^4;			//用于切换数码管显示模式

bit time_flag = 0;
bit beep_active = 0;
bit uart_cmd_ready = 0;
bit alarm_enabled = 0;  							//闹钟使能标志

u8 beep_state = 0;
u8 i = 0;
u8 uart_buf[UART_BUF_LEN];
u8 uart_buf_idx = 0;
u8 led_pwm_duty[4];   								//4个LED的PWM占空比
u8 led_pwm_counter = 0;               //PWM计数器（0-99）
u8 fade_direction[4];                 //0渐暗, 1渐亮
u8 active_led = 0;                    //当前活动的LED
u16 led_delay_count = 0;
u16 main_loop_counter = 0;
u16 beep_duration = 0;  							//蜂鸣器持续时间（毫秒）
u8 beep_freq_counter = 0;
u8 beep_freq_target = 0;

//函数声明
void UART_SendChar(char ch);
void UART_SendString(char *str);
void UART_ExceptYear(int num);
void UART_Year(int Year);
void UART_SendAllMessage(void);
void Date_Update(void);
void UART_ParseCmd(void); 		 				//解析串口指令


void delay_10us_loop(u8 n) {
    while(n--) {
        _nop_(); _nop_(); _nop_(); _nop_(); _nop_();
        _nop_(); _nop_(); _nop_(); _nop_(); _nop_();
    }
}

void generate_tone(u8 freq_index) {
    u8 i, j;
    u8 delay_val;
    
    switch(freq_index) {
        case 1:
            delay_val = 25;
            for(j = 0; j < 2; j++) {
                BEEP = ~BEEP;
                delay_10us_loop(delay_val);
            }
            break;
            
        case 2:
            delay_val = 17;
            for(j = 0; j < 2; j++) {
                BEEP = ~BEEP;
                delay_10us_loop(delay_val);
            }
            break;
            
        case 3:
            delay_val = 12;
            for(j = 0; j < 2; j++) {
                BEEP = ~BEEP;
                delay_10us_loop(delay_val);
            }
            break;
    }
}

//数码管显时模块
void smg_display (DisplayMode current_mode) {
    u8 num[4];
		static u8 display_count = 0;
    static u8 current_digit = 0;
    if (current_mode == display_time) {
        num[0] = hour / 10;
        num[1] = hour % 10;
        num[2] = min / 10;
        num[3] = min % 10;
    }
		else if (current_mode == display_date) {
        num[0] = month / 10;
        num[1] = month % 10;
        num[2] = day / 10;
        num[3] = day % 10;
    }

		if(display_count >= 2) {
        display_count = 0;
        SEL0 = 1;
        SEL1 = 1;
        SEL2 = 1;
        SEL3 = 1;

        SMG_ADP = gsmg_code[num[current_digit]];

        switch(current_digit) {
            case 0: SEL0 = 0; break;
            case 1: SEL1 = 0; break;
            case 2: SEL2 = 0; break;
            case 3: SEL3 = 0; break;
        }

        current_digit++;
        if(current_digit >= 4) {
            current_digit = 0;
        }
		} 
		else{
       display_count++;
		}
}

//中断器初始化
static void InitInterrupt() {
	ET0 = 1;
	ES = 1;
	EA = 1;
}

DisplayMode current_mode = display_time;

//定时器初始化
void InitTimer0(void) {
  TMOD = 0x21;
  TH0 = 0xFC;
  TL0 = 0x18;
  TR0 = 1;
}

//按键3检测
u8 KEY3_Detect(void) {
    static u8 key_not_pressed = 1;
    if(key_not_pressed && (KEY3 == 0)) {
        delay_10us(100);
        if(KEY3 == 0) {
           key_not_pressed = 0;
           return 1;
        }
    }
    else if(KEY3 == 1) {
        key_not_pressed = 1;
    }
    return 0;
}

//切换显示模式
void DisplayMode_Switch(void) {
    if(current_mode == display_time) {
        current_mode = display_date;
    }
    else {
        current_mode = display_time;
    }
}

//串口初始化
static void InitUART(void) {				//4800波特率（开倍增0X80,不开0X00）
		SCON = 0X50;
		PCON = 0X80;
		TL1  = 0XF3;
		TH1  = TL1;
		TR1  = 1;
}

//日期更新逻辑
void Date_Update(void) {
    static const u8 max_days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
		
    if(day > max_days[month-1]) {
        day = 1;
        month++;
        if(month > 12) {
            month = 1;
            year++;
        }
    }
}

//EEPROM读取
void IAP_Assignment(void){
	sec = IAPByteRead(0x2000);
	min = IAPByteRead(0x2001);
	hour = IAPByteRead(0x2002);
	day = IAPByteRead(0x2003);
	month = IAPByteRead(0x2004);
	year = IAPByteRead(0x2005) * 256 + IAPByteRead(0x2006);
	weekflag = IAPByteRead(0x2007);
}

//EEPROM写入
void IAP_Write(void){
	IAPByteWrite(0x2000,sec); 
	IAPByteWrite(0x2001,min); 
	IAPByteWrite(0x2002,hour); 
	IAPByteWrite(0x2003,day); 
	IAPByteWrite(0x2004,month); 
	IAPByteWrite(0x2005, year / 256);    // 高字节
  IAPByteWrite(0x2006, year % 256);    // 低字节
	IAPByteWrite(0x2007,weekflag); 
}

u8 data_valid=0;


//主函数
void main(void) {
		WDT_INIT();           //放狗
    WDT_FEED();           //喂狗
	
		SMG_Init();
		InitTimer0();
		InitUART();
		InitInterrupt();
	
		BEEP = 1;
		alarm_enabled = 0;
	
		//验证数据有效性
		data_valid = IAPByteRead(0x2200);
		if(data_valid == 0x01) {
        IAP_Assignment();
        UART_SendString("Restored time from EEPROM\r\n");
    }
		else {
        UART_SendString("Using preset time\r\n");
        IAPSectorErase(0x2000);
        IAP_Write();
			
        IAPSectorErase(0x2200);
				IAPByteWrite(0x2200, 0x01);
    }
		
		
		LED1 = LED2 = LED3 = LED4 = 1;
		for(i = 0; i < 4; i++) {
			 led_pwm_duty[i] = 0;
			 fade_direction[i] = 0;
		}
		active_led = 0;
		
		
    while(1){
			main_loop_counter++;
 			smg_display(current_mode);
			
			if(uart_cmd_ready) {
				uart_cmd_ready = 0;
				UART_ParseCmd();
				UART_SendAllMessage();
			}


			//LED流水灯（PWM改进）
			if(++led_delay_count >= FADE_INTERVAL) {
         led_delay_count = 0;
            
         for(i = 0; i < 4; i++) {
             if(i == active_led) {
                if(fade_direction[i] == 0) {
                   if(led_pwm_duty[i] < (PWM_PERIOD - 1)) {
                       led_pwm_duty[i] += FADE_STEP;
                   }
									 else 
									 {
                      fade_direction[i] = 1;
                   }
                }
								else 
								{
                   if(led_pwm_duty[i] > 0) {
                       led_pwm_duty[i] -= FADE_STEP;
                   }
									 else 
									 {
                       fade_direction[i] = 0;
                       active_led = (active_led + 1) % 4;
                   }
                }
              }
						  else
							{
                if(led_pwm_duty[i] > 0) {
                    if(led_pwm_duty[i] > 2) {
                        led_pwm_duty[i] -= 2;
										}
                } 
								else 
								{
                    led_pwm_duty[i] = 0;
                }
              }
          }
			}//以上。

			//时间处理标志位
			if(time_flag == 1){
				time_flag=0;
				UART_SendAllMessage();
	      WDT_DISABLE();  		//拴狗
				IAPSectorErase(0x2000);           //擦除起始地址为0x2000的扇区
				IAP_Write();         						  //对起始地址为0x2000的扇区写入寄存器的值
				WDT_ENABLE();   		//解狗绳
			}
			
			//蜂鸣器闹钟模块（加入变调）
			if(alarm_enabled && keyflag == 0 && !beep_active) {
				 if(hour == alarm_hour && min == alarm_min && sec >= alarm_sec) {
						beep_active = 1;
						beep_duration = 0;
						beep_state = 0;
						beep_freq_target = 1;
				 }
			 }
			if(beep_active) {
					if(beep_duration < BEEP_TONE_CHANGE_MS) {
							beep_freq_target = 1;  //低音
					}
					else if(beep_duration < (BEEP_TONE_CHANGE_MS * 2)) {
							WDT_FEED();           //喂狗
							beep_freq_target = 2;  //中音
					}
					else {
							beep_freq_target = 3;  //高音
						
					}
					generate_tone(beep_freq_target);
					beep_duration++;
					if(beep_duration >= BEEP_TOTAL_DURATION) {
							beep_active = 0;
							BEEP = 1;
							keyflag = 1;
					}
					//按键停止
					if(KEY1 == 0) {
							delay_10us(1000);
							if(KEY1 == 0) {
									beep_active = 0;
									BEEP = 1;
									keyflag = 1;
									while(KEY1 == 0) {
										 smg_display(current_mode);
									}		
							}
					 }		
      }//以上。
			main_loop_counter++;
				//KEY3检测
				if(KEY3_Detect()) {
					DisplayMode_Switch();
				}
		}
}



//定时器中断
void Timer0_Interrupt(void) interrupt 1 {
    static u16 count = 0;
		static u8 pwm_count = 0;
		static u16 wdt_count = 0;
	
    TH0 = 0xFC;
    TL0 = 0x18;
	
		//看门狗（主循环正常才喂狗）
		if(++wdt_count >= 100) {
       wdt_count = 0;
    
			 if(main_loop_counter < 2) {
				 
			 }
			 else {
        main_loop_counter = 0;
        WDT_FEED();
			 }
    }//以上。
	
    count++;
	
    if(count >= 1000) {
      count = 0;
			sec++;
			if(sec >= 60){
				sec=0;
				min++;
				if(min >= 60){
					min=0;
					hour++;
					if(hour >= 24){
						hour=0;
						day++;
						weekflag++;
            if(day > max_days[month-1]) {
               day = 1;
               month++;
               if(month > 12) {
                  month = 1;
                  year++;
							 }
						}
					}
				}
			}
			time_flag=1;
    }
		//LED呼吸灯实现
		pwm_count++;
    if(pwm_count >= PWM_PERIOD) {
        pwm_count = 0;
    }
    LED1 = (led_pwm_duty[0] > pwm_count) ? 0 : 1;
    LED2 = (led_pwm_duty[1] > pwm_count) ? 0 : 1;
    LED3 = (led_pwm_duty[2] > pwm_count) ? 0 : 1;
    LED4 = (led_pwm_duty[3] > pwm_count) ? 0 : 1;
}


//串口中断
void UART_Handler() interrupt 4 {
		u8 ch;
		if (RI){
		
		ch = SBUF;
		if(uart_buf_idx < UART_BUF_LEN-1 && ch != '\n') {
			uart_buf[uart_buf_idx++] = ch;
		}
		if(ch == '\n') {
			uart_buf[uart_buf_idx] = '\0';
			uart_buf_idx = 0;
			uart_cmd_ready = 1;
		}
		
		RI=0;
	}
}

//串口打印（简化）
void UART_Print(char *str) {
    while (*str) {
        UART_SendChar(*str);
        str++;
    }
}

//串口发送字符
void UART_SendChar(char ch){
		SBUF = ch;
    while(!TI){
       smg_display(current_mode);
    }
    TI = 0; 
}

//串口发送字符串
void UART_SendString(char *str){
		while(*str != '\0'){
			UART_SendChar(*str);
			str++;
		}
}

//串口发送年份以外信息（月日，十分，星期几）
void UART_ExceptYear(int num){
		UART_SendChar((num / 10) + '0');
    UART_SendChar((num % 10) + '0');
}

//串口发送年份
void UART_Year(int Year){
		UART_SendChar((Year / 1000) % 10 + '0');
    UART_SendChar((Year / 100) % 10 + '0');
    UART_SendChar((Year / 10) % 10 + '0');
    UART_SendChar(Year % 10 + '0');
}

//串口打包发送全部时间信息
void UART_SendAllMessage(void){
		UART_Year(year);
    UART_SendChar('-');
	
		UART_ExceptYear(month);
    UART_SendChar('-');
	
    UART_ExceptYear(day);
    UART_SendString("  "); 
	
		UART_ExceptYear(hour);
		UART_SendChar(':');
	
    UART_ExceptYear(min);
    UART_SendChar(':');
	
		UART_ExceptYear(sec);
    UART_SendChar(' ');
	
    UART_SendString(weekdays[(weekflag + 3) % 7]);				//根据2026年历
    UART_SendChar('\r');
	
    UART_SendChar('\n');
		
}

//更改时间指令解析（格式：set:YYYY-MM-DD HH:MM:SS）
//设置闹钟指令解析（格式：alarm:HH-MM-SS）
void UART_ParseCmd(void) {
    if (strncmp(uart_buf, "set:", 4) == 0) 
    {
        year = (uart_buf[4]-'0')*1000 + (uart_buf[5]-'0')*100 + (uart_buf[6]-'0')*10 + (uart_buf[7]-'0');
        month = (uart_buf[9]-'0')*10 + (uart_buf[10]-'0');
        day = (uart_buf[12]-'0')*10 + (uart_buf[13]-'0');
        hour = (uart_buf[15]-'0')*10 + (uart_buf[16]-'0');
        min = (uart_buf[18]-'0')*10 + (uart_buf[19]-'0');
        sec = (uart_buf[21]-'0')*10 + (uart_buf[22]-'0');
        
        UART_SendString("Time updated\r\n");
    }
    else if (strncmp(uart_buf, "alarm:", 6) == 0) 
    {
        alarm_hour = (uart_buf[6]-'0')*10 + (uart_buf[7]-'0');
        alarm_min = (uart_buf[9]-'0')*10 + (uart_buf[10]-'0');
        alarm_sec = (uart_buf[12]-'0')*10 + (uart_buf[13]-'0');
        alarm_enabled = 1;
        keyflag = 0;
        
        UART_SendString("Alarm set to: ");
        UART_ExceptYear(alarm_hour);
        UART_SendChar(':');
        UART_ExceptYear(alarm_min);
        UART_SendChar(':');
        UART_ExceptYear(alarm_sec);
        UART_SendString("\r\n");
    }
}