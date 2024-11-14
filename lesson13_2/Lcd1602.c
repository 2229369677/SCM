#include <REG52.H>


#define LCD1602_DB P0
sbit LCD1602_RS = P1^0;
sbit LCD1602_RW = P1^1;
sbit LCD1602_E = P1^5;

//等待液晶准备好
void LcdWaitReady(){
	unsigned char sta;
	LCD1602_DB = 0xff;
	LCD1602_RS = 0;
	LCD1602_RW = 1;
	do{
		LCD1602_E = 1;
		sta = LCD1602_DB;
		LCD1602_E = 0;
	}while(sta&=0x80);
}


//写命令
void LcdWriteCmd(unsigned char cmd){
	LcdWaitReady();//准备好
	LCD1602_RS = 0;
	LCD1602_RW = 0;
	LCD1602_DB = cmd;
	LCD1602_E = 1;
	LCD1602_E = 0;
}

//数据坐标初始化
void LcdSetCursor(unsigned char x,unsigned char y){
	unsigned char addr;
	if(y == 0){
		addr = 0x00+x;
	}
	else{
		addr = 0x40+x;
	}
	LcdWriteCmd(addr|0x80);//初始化要+0x80；
}

void LcdWriteDat(unsigned char dat){
	//准备好
	LcdWaitReady();
	
	LCD1602_RS = 1;
	LCD1602_RW = 0;
	LCD1602_DB = dat;
	LCD1602_E = 1;
	LCD1602_E = 0;
}

//在液晶上显示str
void LcdShowStr(unsigned char x,unsigned char y,unsigned char *str){
	//设置坐标
	LcdSetCursor(x,y);
	//一个字节一个字节输入
	while(*str != '\0'){
		LcdWriteDat(*str++);
	}
}

//部分清除
void LcdAreaClear(unsigned char x, unsigned char y, unsigned char len){
	LcdSetCursor(x,y);
	while(len--){
		LcdWriteDat(' ');//写入空格就是清除
	}
}

void InitLcd1602(){
	LcdWriteCmd(0x38);//16*2，5*7，8位数据接口
	LcdWriteCmd(0x0c);//显示器开，光标关闭
	LcdWriteCmd(0x06);//文字不动，地址加1
	LcdWriteCmd(0x01);//清屏
}