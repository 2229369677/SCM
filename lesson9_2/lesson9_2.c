#include <REG52.H>

unsigned long beats= 0;
void StartMotor(unsigned long angle);
void main(){
	
	EA = 1;
	TMOD = 0x01;
	TH0 = 0xf8;
	TL0 = 0xcd;
	ET0 = 1;
	TR0 = 1;
	
	StartMotor(360*2);
	while(1);
}

void StartMotor(unsigned long angle){
	EA = 0;
	beats = (angle*4096)/360;
	EA = 1;
}

void InterruptTime0() interrupt 1{
	unsigned char temp;
	static unsigned char index = 0;
	unsigned char code BeatCode[8] = {  //步进电机节拍对应的IO控制代码
        0xE, 0xC, 0xD, 0x9, 0xB, 0x3, 0x7, 0x6
  };
	TH0 = 0xf8;
	TL0 = 0xcd;
	if(beats != 0){
		temp = P1;
		temp&=0xf0;
		temp|=BeatCode[index++];
		P1 = temp;
		index&=0x07;
		beats--;
	}
	else{
		P1|=0x0f;
	}
}