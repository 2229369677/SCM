//接收并解析NEC协议的红外编码，并将用户码喝键码显示到数码管上

#include <REG52.H>

sbit ADDR3 = P1^3;
sbit ENLED = P1^4;

unsigned char code LedChar[] = {
    0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8,
    0x80, 0x90, 0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E
};

unsigned char LedBuff[6] = {  //数码管显示缓冲区
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};
unsigned char T0RH = 0;
unsigned char T0RL = 0;

extern bit irflag;//红外信号标志
extern unsigned char ircode[4];
extern void InitInfrared();
void ConfigTimer0(unsigned int ms);

void main(){
	EA = 1;
	ENLED = 0;
	ADDR3  =1;
	InitInfrared();//初始化红外功能
	ConfigTimer0(1);
	PT0 = 1;//配置T0中断为高优先级，可以消除接收时的闪烁
	
	while(1){
		if(irflag){
			irflag = 0;
			LedBuff[5] = LedChar[ircode[0] >> 4];
			LedBuff[4] = LedChar[ircode[0]&0x0F];
			LedBuff[1] = LedChar[ircode[2] >> 4];
			LedBuff[0] = LedChar[ircode[2]&0x0F];
		}
	}
}


void ConfigTimer0(unsigned int ms){
	unsigned char tmp;
	
	tmp = 11059200/12;
	tmp = (tmp*ms)/1000;
	tmp = 65536-tmp;
	tmp = tmp+13;
	T0RH = (unsigned char)(tmp>>8);
	T0RL = (unsigned char)tmp;
	TMOD &=0xf0;
	TMOD |=0x01;
	TH0 = T0RH;
	TL0 = T0RL;
	ET0 = 1;
	TR0 = 1;
}

void LedScan(){
	static unsigned char i = 0;
	
	P0 = 0xff;
	P1 = (P1&0xf8)|i;//位选
	P0 = LedBuff[i];
	if(i<sizeof(LedBuff)-1)
		i++;
	else
		i = 0;
}

void InterruptTimr0() interrupt 1{
	TH0 = T0RH;
	TL0 = T0RL;
	LedScan();//数码管扫描显示
}