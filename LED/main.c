#include <REG52.H>

sbit LED = P0^0;
sbit ADDR0 = P1^0;
sbit ADDR1 = P1^1;
sbit ADDR2 = P1^2;
//
sbit ADDR3 = P1^3;
sbit ENLED = P1^4;

void main(){
	ENLED = 0;
	ADDR3 = 1;
	//
	ADDR2 = 1;
	ADDR1 = 1;
	ADDR0 = 0;
	//11111101
	LED = 1;
	while(1);
}