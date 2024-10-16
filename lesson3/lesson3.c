#include <REG52.H>

sbit LED = P0^1;
sbit ADDR0 = P1^0;
sbit ADDR1 = P1^1;
sbit ADDR2 = P1^2;
sbit ADDR3 = P1^3;
sbit ENLED = P1^4;


void main(){
	unsigned int i;
	ADDR3 = 1;
	ENLED = 0;
	//
	ADDR0 = 0;
	ADDR1 = 1;
	ADDR2 = 1;
	
	while(1){
		LED = 0;
		for(i = 0;i<30000;i++);
		LED = 1;
		for(i = 0;i<30000;i++);
	}
}
	