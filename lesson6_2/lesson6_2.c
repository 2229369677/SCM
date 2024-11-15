#include <REG52.H>


sbit ADDR0 = P1^0;
sbit ADDR1 = P1^1;
sbit ADDR2 = P1^2;
sbit ADDR3 = P1^3;
sbit ENLED = P1^4;

unsigned char code LedChar[] = {
	0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,
	0x88,0x83,0xc6,0xa1,0x86,0x8e};
unsigned char LedBuff[6] = {
	0xff,0xff,0xff,0xff,0xff,0xff};
unsigned char flag1s = 0;
unsigned char i;
unsigned int cnt; 
void main(){
	unsigned long sec;
	
	ADDR3 = 1;
	ENLED = 0;
	
	TMOD = 0x01;
	TH0 = 0xfc;
	TL0 = 0x67;
	TR0 = 1;
	
	EA = 1;
	ET0 = 1;
	
	while(1){
		if(TF0 == 1){
			TF0 =0;
			if(flag1s){
				flag1s = 0;
				sec++;
				LedBuff[0] = LedChar[sec%10];
				LedBuff[1] = LedChar[sec/10%10];
				LedBuff[2] = LedChar[sec/100%10];
				LedBuff[3] = LedChar[sec/1000%10];
				LedBuff[4] = LedChar[sec/10000%10];
				LedBuff[5] = LedChar[sec/100000%10];
			}
		}
	}
}
			
void InterruptTime0() interrupt 1{
	TH0 = 0xfc;
	TL0 = 0x67;
	cnt++;
	if(cnt>=1000){
		cnt = 0;
		flag1s = 1;
	}
	P0 = 0xff;
	switch(i){
		case 0:ADDR2 = 0;ADDR1 = 0;ADDR0 = 0;i++;P0 = LedBuff[0];break;
		case 1:ADDR2 = 0;ADDR1 = 0;ADDR0 = 1;i++;P0 = LedBuff[1];break;
		case 2:ADDR2 = 0;ADDR1 = 1;ADDR0 = 0;i++;P0 = LedBuff[2];break;
		case 3:ADDR2 = 0;ADDR1 = 1;ADDR0 = 1;i++;P0 = LedBuff[3];break;
		case 4:ADDR2 = 1;ADDR1 = 0;ADDR0 = 0;i++;P0 = LedBuff[4];break;
		case 5:ADDR2 = 1;ADDR1 = 0;ADDR0 = 1;i=0;P0 = LedBuff[5];break;
		default:break;
	}
}