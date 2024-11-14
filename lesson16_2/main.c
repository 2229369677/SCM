//控制DS18B20测量温度并将温度值显示到液晶上
#include <REG52.H>

bit flagls = 0;//1s定时标志
unsigned char T0RH = 0;
unsigned char T0RL = 0;


void ConfigTimer0(unsigned int ms);
unsigned char IntToString(unsigned char *str, int dat);
extern bit Start18B20();
extern bit Get18B20Temp(int *temp);
extern void InitLcd1602();
extern void LcdShowStr(unsigned char x, unsigned char y, unsigned char *str);


void main(){
	bit res;
	int temp;//读取到当前温度值
	int intT,decT;//温度值的整数和小数
	unsigned char len;
	unsigned char str[12];
	
	EA = 1;
	ConfigTimer0(10);//10ms
	Start18B20();//启动18B20
	InitLcd1602();
	
	while(1){
		if(flagls){//每秒更新一次温度
			flagls = 0;
			res = Get18B20Temp(&temp);//读取当前温度
			if(res){//读取成功时，刷新当前温度显示
				intT = temp>>4;
				decT = temp&0xf;
				len = IntToString(str,intT);//整数部分转换成字符串
				str[len++] = '.';
				decT = (decT*10)/16;//取一位小数
				str[len++] = decT+'0';
				while(len<6){
					str[len++] = ' ';
				}
				str[len] = '\0';
				LcdShowStr(0,0,str);
			}else{
				LcdShowStr(0,0,"error!");
			}
			Start18B20();
		}
	}
}

unsigned char IntToString(unsigned char *str,int dat){
	signed char i = 0;
	unsigned char len = 0;
	unsigned char buf[6];
	
	if(dat<0){
		dat = -dat;
		*str++ = '-';
		len++;
	}
	do{
		buf[i++] = dat%10;
		dat/=10;
	}while(dat>0);
	len+=i;
	while(i-->0){
		*str++ = buf[i]+'0';
	}
	*str = '\0';
	
	return len;
}

void ConfigTimer0(unsigned int ms){
	unsigned long tmp;
	
	tmp = 11059200/12;
	tmp = (tmp*ms)/1000;
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
	static unsigned char tmrls= 0;
	TH0 = T0RH;
	TL0 = T0RL;
	tmrls++;
	if(tmrls>=100){
		tmrls = 0;
		flagls =1;
	}
}