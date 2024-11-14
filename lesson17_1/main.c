#include <REG52.H>

bit flag300ms = 1;//300ms定时标志
unsigned char T0RH = 0;
unsigned char T0RL = 0;

void ConfigTimer0(unsigned int ms); 
unsigned char GetADCValue(unsigned char chn); 
void ValueToString(unsigned char *str, unsigned char val); 
extern void I2CStart(); 
extern void I2CStop(); 
extern unsigned char I2CReadACK(); 
extern unsigned char I2CReadNAK(); 
extern bit I2CWrite(unsigned char dat); 
extern void InitLcd1602(); 
extern void LcdShowStr(unsigned char x, unsigned char y, unsigned char *str);

void main(){
	
	unsigned char val;
	unsigned char str[10];
	EA = 1;//开总中断
	ConfigTimer0(10);
	InitLcd1602();
	LcdShowStr(0,0,"AIN0  AIN1  AIN3");
	while(1){
		if (flag300ms) 
		{ 
			flag300ms = 0; 
			//显示通道 0 的电压 
			 val = GetADCValue(0); //获取 ADC 通道 0 的转换值 
			 ValueToString(str, val); //转为字符串格式的电压值 
			 LcdShowStr(0, 1, str); //显示到液晶上 
			 //显示通道 1 的电压 
			 val = GetADCValue(1); 
			 ValueToString(str, val); 
			 LcdShowStr(6, 1, str); 
			 //显示通道 3 的电压 
			 val = GetADCValue(3); 
			 ValueToString(str, val); 
			 LcdShowStr(12, 1, str); 
		} 
	}
}

//读取当前的ADC转换值,chn-ADC通道号0~3
unsigned char GetADCValue(unsigned char chn){
	unsigned char val;
	
	I2CStart();
	if(!I2CWrite(0x48<<1)){//寻址
		I2CStop();
		return 0;
	}
	//0b01000000
	I2CWrite(0x40|chn);//写入控制字节
	I2CStart();
	I2CWrite((0x48<<1)|0x01);//寻址，指定后续为读操作
	I2CReadACK();//先空读一个字节,提供采样转换时间
	val = I2CReadNAK();//读取刚刚转换完的值
	I2CStop();
	return val;
}

void ValueToString(unsigned char *str, unsigned char val){
	//电压值=转换结果*2.5V/255，式中的 25 隐含了一位十进制小数 
	 val = (val*25) / 255; 
	 str[0] = (val/10) + '0'; //整数位字符 
	 str[1] = '.'; //小数点 
	 str[2] = (val%10) + '0'; //小数位字符 
	 str[3] = 'V'; //电压单位 
	 str[4] = '\0'; //结束符
}
void ConfigTimer0(int ms){
	unsigned long tmp;
	
	tmp = 11059200/12;
	tmp = (tmp*ms)/1000;
	tmp = 65536-tmp;
	tmp = tmp+12;
	
	T0RH = (unsigned char)(tmp>>8);
	T0RL = (unsigned char)tmp;
	TMOD &= 0xf0;
	TMOD |= 0x01;
	TH0 = T0RH;
	TL0 = T0RL;
	ET0 = 1;
	TR0 = 1;
}

void InterruptTimer0() interrupt 1{
	static unsigned char tmr300ms = 0;
	
	TH0 = T0RH;
	TL0 = T0RL;
	tmr300ms++;
	if(tmr300ms>=30){
		tmr300ms = 0;
		flag300ms = 1;
	}
}