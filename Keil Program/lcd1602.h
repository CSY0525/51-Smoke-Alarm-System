#ifndef __LCD1602_H_
#define __LCD1602_H_
#include "reg51.h"

typedef unsigned char u8;
typedef unsigned int u16;
typedef unsigned char uint8;
typedef unsigned int uint16;

/**********************
引脚别名定义
***********************/		
#define LCDDATA P0
sbit lcd1602_rs=P2^4;        //1602的数据/指令选择控制线 
sbit lcd1602_rw=P2^5;        //1602的读写控制线 
sbit lcd1602_en=P2^6;        //1602的使能控制线 
void delay_ms(unsigned int ms);
void lcd_wcom(uint8 com);
void lcd_wdat(uint8 dat); 
void lcd_init(void);
void DisplayOneChar(uint8 X, uint8 Y, uint8 DData);
void DisplayListChar(uint8 X, uint8 Y, uint8 code *DData);
bit LCD_Check_Busy(void);
#endif
