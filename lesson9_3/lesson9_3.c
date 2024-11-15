#include <REG52.H>
//通过按钮来控制步进电机

sbit KeyIn1 = P2^4;
sbit KeyIn2 = P2^5;
sbit KeyIn3 = P2^6;
sbit KeyIn4 = P2^7;
sbit KeyOut1 = P2^3;
sbit KeyOut2 = P2^2;
sbit KeyOut3 = P2^1;
sbit KeyOut4 = P2^0;

unsigned char code KeyCodeMap[4][4] = { 
    { 0x31, 0x32, 0x33, 0x26 }, 
    { 0x34, 0x35, 0x36, 0x25 }, 
    { 0x37, 0x38, 0x39, 0x28 }, 
    { 0x30, 0x1B, 0x0D, 0x27 }  
};

unsigned char KeySta[4][4] = {  //全部按键的当前状态
    {1, 1, 1, 1},  {1, 1, 1, 1},  {1, 1, 1, 1},  {1, 1, 1, 1}
};
signed long beats = 0;

void KeyDriver();

void main(){
	EA = 1;
	TMOD = 0x01;
	TH0 = 0xfc;
	TL0 = 0x67;
	ET0 = 1;
	TR0 = 1;
	while(1){
		KeyDriver();
	}
}

void StartMotor(signed long angle){
	EA = 0;
	beats = (angle*4076)/360;
	EA = 1;
}

void StopMotor(){
	EA = 0;
	beats = 0;
	EA = 1;
}

void KeyAction(unsigned char keycode){
	static bit dirMotor = 0;//转动方向
	
	if((keycode>=0x30)&&(keycode<=0x39)){
		if(dirMotor==0){
			StartMotor(360*(int)(keycode-0x30));
		}
		else{
			StartMotor(-360*(int)(keycode-0x30));
		}
	}
	else if(keycode==0x26){
		dirMotor = 0;
	}
	else if(keycode==0x28){
		dirMotor = 1;
	}
	else if(keycode==0x25){
		StartMotor(90);
	}
	else if(keycode==0x27){
		StartMotor(-90);
	}
	else if(keycode==0x1b){
		StopMotor();
	}
}

void KeyDriver(){
	unsigned char i,j;
	static unsigned char backup[4][4] = {
		{1,1,1,1},{1,1,1,1},{1,1,1,1},{1,1,1,1}
	};
	for(i = 0;i<4;i++){
		for(j = 0;j<4;j++){
			if(backup[i][j]!=KeySta[i][j]){
				if(backup[i][j]!=0){
					KeyAction(KeyCodeMap[i][j]);//调用按键动作函数
				}
				backup[i][j] = KeySta[i][j];
			}
		}
	}
}

void KeyScan(){
	unsigned char i;
	static unsigned char keyout = 0;
	static unsigned char keybuf[4][4] = {  
        {0xFF, 0xFF, 0xFF, 0xFF},  {0xFF, 0xFF, 0xFF, 0xFF},
        {0xFF, 0xFF, 0xFF, 0xFF},  {0xFF, 0xFF, 0xFF, 0xFF}
	};
	
	keybuf[keyout][0] = (keybuf[keyout][0]<<1)|KeyIn1;
	keybuf[keyout][1] = (keybuf[keyout][1]<<1)|KeyIn2;
	keybuf[keyout][2] = (keybuf[keyout][2]<<1)|KeyIn3;
	keybuf[keyout][3] = (keybuf[keyout][3]<<1)|KeyIn4;
	
	for(i = 0;i<4;i++){
		if((keybuf[keyout][i] & 0x0f) == 0x00){
			KeySta[keyout][i] = 0;
		}
		else if((keybuf[keyout][i]& 0x0f)==0x0f){
			KeySta[keyout][i] = 1;
		}
	}
	keyout++;
	keyout &=0x03;//到4归0
	switch(keyout){
		case 0:KeyOut4 = 1;KeyOut1 = 0;break;
		case 1:KeyOut1 = 1;KeyOut2 = 0;break;
		case 2:KeyOut2 = 1;KeyOut3 = 0;break;
		case 3:KeyOut3 = 1;KeyOut4 = 0;break;
		default:break;
	}
}

void TurnMotor(){
	unsigned char tmp;  
	static unsigned char index = 0; 
	unsigned char code BeatCode[8] = {
			0xE, 0xC, 0xD, 0x9, 0xB, 0x3, 0x7, 0x6
	};
	if(beats!=0){
		if(beats>0){
			index++;
			beats--;
			index &= 0x07;
		}
		else if(beats<0){
			index--;
			beats++;
			index &= 0x07;
		}
		tmp = P1;
		tmp &= 0xf0;
		tmp |= BeatCode[index];
		P1 = tmp;
	}
	else{
		P1 |= 0x0f;
	}
}

void InterruprTime0() interrupt 1{
	static bit div = 0;
	
	TH0 = 0xfc;
	TL0 = 0x67;
	KeyScan();//按键扫描
	
	div = ~div;
	if(div == 1){
		TurnMotor();//电机转动控制
	}
}