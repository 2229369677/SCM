#include <REG52.H>

sbit BUZZ = P1^6;

bit flagBuzzOn = 0;
unsigned char T0RH = 0;
unsigned char T0RL = 0;

void ConfigTimer0(unsigned int ms);
extern void UartDriver();
extern void ConfigUART(unsigned int baud);	
extern void UartRxMonitor(unsigned char ms);
extern void UartWrite(unsigned char *buf, unsigned char len);
extern void InitLcd1602();
extern void LcdShowStr(unsigned char x, unsigned char y, unsigned char *str);
extern void LcdAreaClear(unsigned char x, unsigned char y, unsigned char len);

void main(){
	EA = 1;           
	ConfigTimer0(1);  
	ConfigUART(9600);
	InitLcd1602();    
	
	while (1)
	{
	    UartDriver();
	}
}


void ConfigTimer0(unsigned int ms){
	unsigned int temp;
	
	temp = 11095200/12;
	temp = (temp*ms)/1000;
	temp = 65536-temp;
	temp+=33;//补偿
	
	T0RH = (unsigned char)(temp>>8);
	T0RL = (unsigned char)temp;
	
	TMOD &=0x0f;
	TMOD |= 0x01;
	TH0 = T0RH;
	TL0 = T0RL;
	ET0 = 1;
	TR0 = 1;
}

bit CmpMemory(unsigned char *ptr1,unsigned char *ptr2,unsigned char len){
	while(len--){
		if(*ptr1++ != *ptr2++){
			return 0;
		}
	}
	return 1;
}

void UartAction(unsigned char *buf, unsigned char len){
	unsigned char i;
	unsigned char code cmd0[] = "buzz on"; 
	unsigned char code cmd1[] = "buzz off";
	unsigned char code cmd2[] = "showstr ";
	
	unsigned char code cmdLen[] = {
		sizeof(cmd0)-1,sizeof(cmd1)-1,sizeof(cmd2)-1
	};
	
	unsigned char code *cmdPtr[] = {
		&cmd0[0],&cmd1[0],&cmd2[0]
	};
	
	for(i = 0;i<sizeof(cmdLen);i++){
		if(len>=cmdLen[i]){//长度比较
			if(CmpMemory(buf,cmdPtr[i],cmdLen[i])){
				break;
			}
		}
	}
	switch(i){
		case 0:{
			flagBuzzOn = 1;
			break;
		}
		case 1:{
			flagBuzzOn = 0;
			break;
		}
		case 2:{
			//要显示后面的字符串
			buf[len] = '\0';
			LcdShowStr(0,0,buf+cmdLen[2]);
			i = len-cmdLen[2];
			if(i<16){
				LcdAreaClear(i,0,16-i);//>16，清除后面的字符
			}
			break;
		}
		default:{
			UartWrite("bad command.\r\n",sizeof("bad command.\r\n")-1);//给上机发送错误命令的提示
			return;
		}
	}
	buf[len++] = '\r';
	buf[len++] = '\n';
	UartWrite(buf,len);
}

void InterruptTimer0() interrupt 1{
	TH0 = T0RH;
	TL0 = T0RL;
	if(flagBuzzOn){
		BUZZ = ~BUZZ;
	}
	else{
		BUZZ  = 1;
	}
	UartRxMonitor(1);//串口接收监控
}