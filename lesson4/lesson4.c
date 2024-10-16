#include <REG52.H>

sbit ADDR0 = P1^0;
sbit ADDR1 = P1^1;
sbit ADDR2 = P1^2;
sbit ADDR3 = P1^3;
sbit ENLED = P1^4;

void main(){
	int i,j;
	unsigned int p = 0xff;
	ENLED = 0;
	ADDR3 = 1;
	ADDR2 = 1;
	ADDR1 = 1;
	ADDR0 = 0;
	while(1){
		for(i = 0;i<8;i++){
			P0 = p^(1<<i);
			for(j = 0;j<30000;j++);
		}
	}
}