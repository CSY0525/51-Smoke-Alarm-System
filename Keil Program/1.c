#include "reg51.h"
#include "stdio.h"
#include "lcd1602.h"
#include "DS18B20.h"

#define u8 unsigned char
#define u16 unsigned int

sbit A1 = P1^0;
sbit A2 = P1^1;
sbit A3 = P1^2;
sbit A4 = P1^3;
sbit BEEP = P1^6;
sbit KEY1 = P2^0;
sbit KEY2 = P2^1;
sbit Smoke = P2^2;

u8 Motor_Tick = 0,Motor_Time = 6;
u8 table1[16] = "",table2[16] = "    SMOKE:YES   ",table3[16] = "    SMOKE:NO    ";
u8 FireTmp = 25;//set fire tmp
float T;
/***********************************/
void Delay(time);
void Motor();
void Timer();
void initT0();
void LCD();
void DS18B20();
void Fire();
u8 YanWu();
/***********************************/
int main(){
	u8 Tick = 1;bit KEY_value = !KEY2;
	initT0();lcd_init();DS18B20_ConvertT();
	Delay(100);
	BEEP = 1;
	KEY1 = 1;KEY2 = 1;
	lcd_wcom(0x01);
	while(1){
		Motor();DS18B20();Fire();//LCD();
		if(KEY1 == 0 && Tick == 1){
			Delay(10);
			Tick = 0;
			if(KEY1 == 0){
				FireTmp++;
				BEEP = 0;Delay(50);BEEP = 1;
				if(FireTmp == 45)
					FireTmp = 20;
			}
		}
		else if(KEY1 == 1)
			Tick = 1;
/***********************************/
		
		if(KEY2){
			Motor_Tick = 1;
		}
		else{
			Motor_Tick = 2;
		}
		if(KEY2==KEY_value){
			Motor_Time=0;
			KEY_value=!KEY_value;
		}		
	}
}

void Fire(){
	u8 i = 0,Tick = 1;
	i = YanWu();
	if(T >= FireTmp && T <= 50 && i == 0 && Tick == 1){
		Delay(20);
		Tick = 0;
		if(T >= FireTmp && T <= 50 && i == 0){
			Motor_Time = 0;BEEP = 0;
		}
	}
	else{
		BEEP = 1;
		Tick = 1;
	}
}

void DS18B20(){
	DS18B20_ConvertT();
	Delay(10);
	T = DS18B20_ReadT() - 4;
}

void LCD(){
	u16 num = T,num1 = FireTmp,i;
	u16 num2 = (unsigned long)(T*100)%100;
	sprintf(table1,"Tmp:%d.%d YZ-%d ",num,num2,num1); 
	for(i = 0;i < 16;i++){
		DisplayOneChar(i,0,table1[i]);
		if(YanWu() == 0)
			DisplayOneChar(i,1,table2[i]);
		else
			DisplayOneChar(i,1,table3[i]);
	}
}

u8 YanWu(){
	if(Smoke == 0){
		return 0;  //YES
	}
	else
		return 1;  //NO
}

void Motor(){
	if(Motor_Tick == 1){
		if(Motor_Time < 5){
			A1 = 1;A2 = 0;A3 = 0;A4 = 0;Delay(5);
			A1 = 0;A2 = 1;A3 = 0;A4 = 0;Delay(5);
			A1 = 0;A2 = 0;A3 = 1;A4 = 0;Delay(5);
			A1 = 0;A2 = 0;A3 = 0;A4 = 1;Delay(5);
		}
		else{
			A1 = 0;A2 = 0;A3 = 0;A4 = 0;
		}
	}
	else if(Motor_Tick == 2){
		if(Motor_Time < 5){
			A1 = 0;A2 = 0;A3 = 0;A4 = 1;Delay(5);
			A1 = 0;A2 = 0;A3 = 1;A4 = 0;Delay(5);
			A1 = 0;A2 = 1;A3 = 0;A4 = 0;Delay(5);
			A1 = 1;A2 = 0;A3 = 0;A4 = 0;Delay(5);
		}
		else{
			A1 = 0;A2 = 0;A3 = 0;A4 = 0;
		}
	}
}

void Timer() interrupt 1{
	u8 num;
	TH0 = (65536 - 10000) / 256;
	TL0 = (65536 - 10000) % 256;
	num++;
	if(num == 200){
		num = 0;
		Motor_Time++;
		LCD();
	}
}

void initT0(){
	TMOD = 0x01;
	TH0 = (65536 - 10000) / 256;
	TL0 = (65536 - 10000) % 256;
	EA = 1;
	ET0 = 1;
	TR0 = 1;
}

void Delay(time){
	u8 i,j;
	for(i = 0;i < time;i++){
		for(j = 0;j < 110;j++);
	}
}