//IIC总线寻址
#include <REG52.H>
#include <intrins.h>

#define I2CDelay() {_nop_();_nop_();_nop_();_nop_();}
sbit I2C_SCL = P3^7;
sbit I2C_SDA = P3^6;

bit I2CAddressing(unsigned char addr);
extern void InitLcd1602();
extern void LcdShowStr(unsigned char x,unsigned char y,unsigned char *str);

void main(){
	bit ack;
	unsigned char str[10];
	
	//初始化液晶
	InitLcd1602();
	
	ack = I2CAddressing(0x50);
	str[0] = '5';
	str[1] = '0';
	str[2] = ':';
	str[3] = (unsigned char)(ack)+'0';
	str[4] = '\0';
	LcdShowStr(0,0,str);
	
	ack = I2CAddressing(0x62);
	str[0] = '6';
	str[1] = '2';
	str[2] = ':';
	str[3] = (unsigned char)(ack)+'0';
	str[4] = '\0';
	LcdShowStr(8,0,str);
	
	while(1);
}

void I2CStart(){
	I2C_SDA = 1;
	I2C_SCL = 1;
	I2CDelay();
	I2C_SDA = 0;
	I2CDelay();
	I2C_SCL = 0;
}

void I2CStop(){
	I2C_SCL = 0;
	I2C_SDA = 0;
	I2CDelay();
	I2C_SCL = 1;
	I2CDelay();
	I2C_SDA = 1;
	I2CDelay();
}

bit I2CWrite(unsigned char dat){
	bit ack;
	unsigned char mask;
	
	for(mask = 0x80;mask!=0;mask>>=1){
		if((mask&dat)==0){
			I2C_SDA = 0;
		}else{
			I2C_SDA = 1;
		}
		I2CDelay();
		I2C_SCL = 1;
		I2CDelay();
		I2C_SCL = 0;
	}
	//
	I2C_SDA = 1;
	I2CDelay();
	I2C_SCL = 1;
	ack = I2C_SDA;
	I2CDelay();
	I2C_SCL = 0;
	
	return ack;
}

bit I2CAddressing(unsigned char addr){
	bit ack;
	I2CStart();
	ack = I2CWrite(addr<<1);
	I2CStop();
	return ack;
}