#include <REG52.H>

sbit ADDR0 = P1^0;
sbit ADDR1 = P1^1;
sbit ADDR2 = P1^2;
sbit ADDR3 = P1^3;
sbit ENLED = P1^4;
sbit Key4 = P2^7;
sbit Key3 = P2^6;
sbit Key2 = P2^5;
sbit Key1 = P2^4;
bit falg01 = 0;
bit falg02 = 0;
unsigned char code LedChar[] = {
	0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
unsigned char LedBuff[6] = {
	0xff,0xff,0xff,0xff,0xff,0xff};
unsigned char count =0;

void main(){
	P2 = 0xf7;
	ADDR3 = 1;
	ENLED = 0;
	TMOD = 0x01;
	TH0 = 0xbc;
	TL0 = 0x67;
	TR0 = 1;
	
	ADDR0 = 0;
	ADDR1 = 0;
	ADDR2 = 0;
	P0 = 0xc0;
	
	EA = 1;
	ET0 = 1;
	
	while(1){
		if(falg01&&falg02){
			falg01 = falg02 = 0;
			count++;
			if(count>=10){
				count = 0;
			}
			P0 = LedChar[count];
		}
	}
}

void InterruptTimer0() interrupt 1{
	static unsigned char state  = 0xff;
	static unsigned char i = 0;
	{
		(state<<1)|Key4;
		if(falg01==0&&Key4==1){
			i++;
		}
		if(falg01==1&&Key4 ==0){
			i++;
		}
		if(falg01==0&&i>=4){
			i = 0;
			falg01 = 1;
		}
		if(falg01==1&&i>=4){
			i=0;
			falg02 = 1;
		}
	}
	TH0 = 0xbc;
	TL0 = 0x67;
}