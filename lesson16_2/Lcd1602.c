#include <REG52.H>

#define LCD1602_DB P0

sbit LCD1602_RS = P1^0;
sbit LCD1602_RW = P1^1;
sbit LCD1602_E = P1^5;

//等待液晶准备
void LcdWaitReady(){
	unsigned char sta;
	LCD1602_DB = 0xff;
	LCD1602_RS = 0;
	LCD1602_RW = 1;
	do{
		LCD1602_E = 1;
		sta = LCD1602_DB;
		LCD1602_E = 0;
	}while(sta&0x80);//bit7==1忙
}

//写指令
void LcdWriteCmd(unsigned char cmd){
	LcdWaitReady();
	LCD1602_RS = 0;
	LCD1602_RW = 0;
	LCD1602_DB = cmd;
	LCD1602_E = 1;
	LCD1602_E = 0;
}


//写入数据
void LcdWriteDat(unsigned char dat){
	LcdWaitReady();
	LCD1602_RS = 1;
	LCD1602_RW = 0;
	LCD1602_DB = dat;
	LCD1602_E = 1;
	LCD1602_E = 0;
}

//设置显示RAM起始地址
void LcdSetCursor(unsigned char x,unsigned char y){
	unsigned char addr;
	if(y==0){
		addr = 0x00+x;
	}else{
		addr = 0x40+x;
	}
	LcdWriteCmd(addr|0x80);
}


//显示
void LcdShowStr(unsigned char x,unsigned char y,unsigned char *str){
	LcdSetCursor(x,y);
	while(*str!='\0'){
		LcdWriteDat(*str++);
	}
}

//初始化液晶
void InitLcd1602(){
	LcdWriteCmd(0x38);
	LcdWriteCmd(0x0c);
	LcdWriteCmd(0x06);
	LcdWriteCmd(0x01);
}