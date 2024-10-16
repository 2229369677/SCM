#include <REG52.H>


sbit ADDR0 = P1^0;
sbit ADDR1 = P1^1;
sbit ADDR2 = P1^2;
sbit ADDR3 = P1^3;
sbit ENLED = P1^4;

unsigned char image[] = {
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x81,0x80,0xFC,0xFC,0x80,0x81,0xFF,
0xC7,0x83,0x81,0xC0,0xC0,0x81,0x83,0xC7,0xFF,0x3C,0x3C,0x00,0x00,0x3C,0x3C,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

void main(){
	
	ADDR3 = 0;
	ENLED = 0;
	
	EA = 1;
	ET0 = 1;
	
	TMOD = 0x01;
	TH0 = 0xfc;
	TL0 = 0x67;
	TR0 = 1;
	while(1);
}

void InterruptTimer0() interrupt 1{
	static unsigned char i = 0;
	static unsigned char index = 0;
	static unsigned char tmr = 0;
	TH0 = 0xfc;
	TL0 = 0x67;
	
	P0 = 0xff;
	switch(i){
		case 0:ADDR2 = 0;ADDR1 = 0;ADDR0 = 0;i++;P0 = image[index+0];break;
		case 1:ADDR2 = 0;ADDR1 = 0;ADDR0 = 1;i++;P0 = image[index+1];break;
		case 2:ADDR2 = 0;ADDR1 = 1;ADDR0 = 0;i++;P0 = image[index+2];break;
		case 3:ADDR2 = 0;ADDR1 = 1;ADDR0 = 1;i++;P0 = image[index+3];break;
		case 4:ADDR2 = 1;ADDR1 = 0;ADDR0 = 0;i++;P0 = image[index+4];break;
		case 5:ADDR2 = 1;ADDR1 = 0;ADDR0 = 1;i++;P0 = image[index+5];break;
		case 6:ADDR2 = 1;ADDR1 = 1;ADDR0 = 0;i++;P0 = image[index+6];break;
		case 7:ADDR2 = 1;ADDR1 = 1;ADDR0 = 1;i=0;P0 = image[index+7];break;
		default:break;
	}
	tmr++;
	if(tmr>=250){
		tmr=0;
		index++;
		if(index>=32){
			index = 0;
		}
	}
}