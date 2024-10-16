#include <REG52.H>

#define LCD1602_DB P0
sbit LCD1602_RS = P1^0;
sbit LCD1602_RW = P1^1;
sbit LCD1602_E = P1^5;

//等待液晶准备
void LcdWaitReady(){
	unsigned char sta;
	LCD1602_DB = 0xFF;
	LCD1602_RS = 0;//指令
	LCD1602_RW = 1;//读
	do{
		LCD1602_E = 1;
		sta = LCD1602_DB;
		LCD1602_E = 0;
	}while(sta&0x80);//状态不忙通过
}


//写入状态
void LcdWriteCmd(unsigned char cmd){
	LcdWaitReady();//准备好
	LCD1602_DB = 0xff;
	LCD1602_RS = 0;
	LCD1602_RW = 0;
	LCD1602_DB = cmd;//准备数据
	LCD1602_E = 1;
	LCD1602_E = 0;
}

//设置数据指针位置
void LcdSetCursor(unsigned char x,unsigned char y){
	unsigned char addr;//数据指针地址
	
	if(y==0){
		addr = 0x00+x;
	}
	else{
		addr = 0x40+x;
	}
	LcdWriteCmd(addr|0x80);//写入指令
}

//写入数据
void LcdWriteDat(unsigned char dat){
	LcdWaitReady();//准备好
	LCD1602_DB = 0xff;
	LCD1602_RS = 1;
	LCD1602_RW = 0;
	LCD1602_DB = dat;
	LCD1602_E = 1;
	LCD1602_E = 0;
}

void LcdShowStr(unsigned char x,unsigned char y,unsigned char*str){
	//数据指针设置
	LcdSetCursor(x,y);
	//显示（连续写入字符）
	while(*str!='\0'){
		LcdWriteDat(*str++);
	}
}

//初始化
void InitLcd1602(){
	LcdWriteCmd(0x38);//16*2显示模式
	LcdWriteCmd(0x0c);//显示器开
	LcdWriteCmd(0x06);//文字不动
	LcdWriteCmd(0x01);//清屏
}

//整屏清除
void LcdFullClear(){
	LcdWriteCmd(0x01);
}

//区域清除
void LcdAreaClear(unsigned char x, unsigned char y, unsigned char len){
	LcdSetCursor(x, y);
	while(len--){
		LcdWriteDat(' ');
	}
}