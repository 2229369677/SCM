//用突发读写模式访问DS1302，并将日期时间显示在液晶上

#include <REG52.H>

sbit DS1302_CE = P1^7;
sbit DS1302_CK = P3^5;
sbit DS1302_IO = P3^4;

bit flag200ms = 0;
unsigned char T0RH;
unsigned char T0RL;

void ConfigTimer0(unsigned int ms);
void InitDS1302();
void DS1302BurstRead(unsigned char *dat);
extern void InitLcd1602();
extern void LcdShowStr(unsigned char x,unsigned char y,unsigned char *str);

void main(){
	unsigned char psec = 0xaa;
	unsigned char time[8];
	unsigned char str[12];
	
	EA = 1;
	ConfigTimer0(1);
	InitDS1302();
	InitLcd1602();
	
	while(1){
		if(flag200ms){
			flag200ms = 0;
			DS1302BurstRead(time);
			if(psec != time[0]){//不一样就更新
				str[0] = '2';
				str[1] = '0';
				str[2] = (time[6]>>4)+'0';//年
				str[3] = (time[6]&0x0f)+'0';
				str[4] = '-';
				str[5] = (time[4]>>4)+'0';//月
				str[6] = (time[4]&0x0f)+'0';
				str[7] = '-';
				str[8] = (time[3] >> 4) + '0';//日
                str[9] = (time[3]&0x0F) + '0';
                str[10] = '\0';
                LcdShowStr(0, 0, str);//显示到液晶的第一行
				
				str[0] = (time[5]&0x0F) + '0'; 
                str[1] = '\0';
                LcdShowStr(11, 0, "week");
                LcdShowStr(15, 0, str);
                
                str[0] = (time[2] >> 4) + '0';//时
                str[1] = (time[2]&0x0F) + '0';
                str[2] = ':';
                str[3] = (time[1] >> 4) + '0';//分
                str[4] = (time[1]&0x0F) + '0';
                str[5] = ':';
                str[6] = (time[0] >> 4) + '0';//秒
                str[7] = (time[0]&0x0F) + '0';
                str[8] = '\0';
                LcdShowStr(4, 1, str);//显示到第二行
				
				psec = time[0];//用当前值更新上次秒数
			}
		}
	}
}

void DS1302ByteWrite(unsigned char dat){
	unsigned char mask;
	for(mask = 0x01;mask!=0;mask<<=1){
		if((mask&dat)!=0){
			DS1302_IO = 1;
		}else{
			DS1302_IO = 0;
		}
		DS1302_CK = 1;
		DS1302_CK = 0;
	}
	DS1302_IO = 1;
}

unsigned char DS1302ByteRead(){
	unsigned char mask;
	unsigned char dat = 0;
	
	for(mask = 0x01;mask!=0;mask<<=1){
		if(DS1302_IO!=0){
			dat|=mask;
		}
		DS1302_CK = 1;
		DS1302_CK = 0;
	}
	return dat;
}

void DS1302SingleWrite(unsigned char reg,unsigned char dat){
	DS1302_CE = 1;
	DS1302ByteWrite((reg<<1)|0x80);
	DS1302ByteWrite(dat);
	DS1302_CE = 0;
}

unsigned char DS1302SingleRead(unsigned char reg){
	unsigned char dat;
	
	DS1302_CE = 1;
	DS1302ByteWrite((reg<<1)|0x81);
	dat = DS1302ByteRead();
	DS1302_CE = 0;
	return dat;
}

//突发模式连续写入8个
void DS1302BurstWrite(unsigned char* dat){
	unsigned char i;
	
	DS1302_CE = 1;
	DS1302ByteWrite(0xBE);
	for(i = 0;i<8;i++){
		DS1302ByteWrite(dat[i]);
	}
	DS1302_CE = 0;
}

//突发模式连续读8个
void DS1302BurstRead(unsigned char*dat){
	unsigned char i;
	
	DS1302_CE = 1;
	DS1302ByteWrite(0xbf);
	for(i = 0;i<8;i++){
		dat[i] = DS1302ByteRead();
	}
	DS1302_CE = 0;
}

void InitDS1302(){
	unsigned char dat;
	unsigned char code InitTime[]={
		0x00,0x00,0x16,0x23,0x10,0x02,0x24,0x00
	};
	
	DS1302_CE = 0;
	DS1302_CK= 0;
	dat = DS1302SingleRead(0);
	if((dat&0x80)!=0){
		DS1302SingleWrite(7,0x00);
		DS1302BurstWrite(InitTime);
	}
}


void ConfigTimer0(unsigned int ms){
	unsigned long tmp;
	
	tmp = 11059200/12;
	tmp = (tmp*ms)/1000;
	tmp = 65536-tmp;
	tmp+=12;
	T0RH = (unsigned char)(tmp>>8);
	T0RL = (unsigned char)tmp;
	
	TMOD &=0xf0;
	TMOD |=0x01;
	TH0 = T0RH;
	TL0 = T0RL;
	ET0 = 1;
	TR0 = 1;
}

void InterruptTime0() interrupt 1{
	static unsigned char tmr200ms = 0;
	TH0 = T0RH;
	TL0 = T0RL;
	tmr200ms++;
	if(tmr200ms>=200){
		tmr200ms = 0;
		flag200ms = 1;
	}
}