#include <REG52.H>

void ConfigUART(unsigned int baud);

void main(){
	EA = 1;
	ConfigUART(9600);
	while(1);
}


void ConfigUART(unsigned int baud){
	SCON = 0x50;
	TMOD &= 0x0f;
	TMOD |= 0x20;//配置T1为模式2
	TH1 = 256-(11059200/12/32)/baud;
	TL1 = TH1;
	ET1 = 0;
	ES = 1;//启用串口中断
	TR1 = 1;
}


void InterruptUART() interrupt 4{
	if(RI){
		RI = 0;
		SBUF +=1;
	}
	if(TI){
		TI = 0;
	}
}