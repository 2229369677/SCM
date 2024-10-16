#include <REG52.H>

sbit LED = P0^0;
sbit ADDR0 = P1^0;
sbit ADDR1 = P1^1;
sbit ADDR2 = P1^2;
sbit ADDR3 = P1^3;
sbit ENLED = P1^4;

void main(){
	unsigned char cnt = 0;
	P0 = 0xffff;
	ENLED = 0;
	ADDR3 = 1;
	ADDR2 = 1;
	ADDR1 = 1;
	ADDR0 = 0;
	TMOD = 0x01;
	//20ms,改变初始值可以设置进位的时间
	//x*12/11059200 = 0.02 x = 18432  T0 = y; y+x = 65536 y = 0xb800;
	TH0 = 0xB8;
	TL0 = 0x00;
	TR0 = 1;
	
	while(1){
		if(TF0==1){
			TF0 = 0;
			TH0 = 0xb8;
			TL0 = 0x00;
			cnt++;
			if(cnt>=50){
				cnt = 0;
				LED = ~LED;
			}
		}
	}
}
		
	