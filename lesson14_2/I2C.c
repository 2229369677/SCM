#include <REG52.H>
#include <intrins.h>

#define I2CDelay() {_nop_();_nop_();_nop_();_nop_();}
sbit I2C_SCL = P3^7;
sbit I2C_SDA = P3^6;

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

//I2C 总线写操作，dat-待写入字节，返回-从机应答位的值
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
		I2C_SCL = 1;//拉高
		I2CDelay();
		I2C_SCL = 0;
	}
	I2C_SDA = 1;//8位发送完后，释放SDA，以检测从机应答
	I2CDelay();
	I2C_SCL = 1;//拉高
	ack = I2C_SDA;
	I2CDelay();
	I2C_SCL = 0;
	
	return (~ack);//取反
}


//I2C总线读取操作，并发送非应答信号，返回读到的字节
unsigned char I2CReadNAK(){
	unsigned char mask;
	unsigned char dat;
	I2C_SDA = 1;//首先确保主机释放SDA
	for(mask = 0x80;mask!=0;mask>>=1){
		I2CDelay();
		I2C_SCL = 1;
		if(I2C_SDA ==0){
			dat &= ~mask;
		}else{
			dat |= mask;
		}
		I2CDelay();
		I2C_SCL = 0;
	}
	//8位发送完毕，主机准备发送应答信号
	I2C_SDA = 1;
	I2CDelay();
	I2C_SCL = 1;
	I2CDelay();
	I2C_SCL = 0;
	
	return dat;
}

//I2C总线读取操作，并发送应答信号，返回读到的字节
unsigned char I2CReadAK(){
	unsigned char mask;
	unsigned char dat;
	I2C_SDA = 1;//首先确保主机释放SDA
	for(mask = 0x80;mask!=0;mask>>=1){
		I2CDelay();
		I2C_SCL = 1;
		if(I2C_SDA ==0){
			dat &= ~mask;
		}else{
			dat |= mask;
		}
		I2CDelay();
		I2C_SCL = 0;
	}
	//8位发送完毕，主机准备发送应答信号
	I2C_SDA = 0;
	I2CDelay();
	I2C_SCL = 1;
	I2CDelay();
	I2C_SCL = 0;
	
	return dat;
}