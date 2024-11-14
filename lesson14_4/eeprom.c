#include <REG52.H>

extern void I2CStart();
extern void I2CStop();
extern unsigned char I2CReadACK();
extern unsigned char I2CReadNAK();
extern bit I2CWrite(unsigned char dat);
	
void E2Read(unsigned char *buf,unsigned char addr,unsigned char len){
	while(1){
		I2CStart();
		if(I2CWrite(0x50<<1)){
			break;
		}
		I2CStop();
	}
	I2CWrite(addr);
	I2CStart();
	I2CWrite((0x50<<1)|0x01);//读
	while(len>1){//先读len-1个
		*buf++ = I2CReadACK();
		len--;
	}
	*buf++ = I2CReadNAK();
	I2CStop();
}

void E2Write(unsigned char *buf,unsigned char addr,unsigned char len){
	while(len>0){
		while(1){
			I2CStart();
			if(I2CWrite(0x50<<1)){
				break;
			}
			I2CStop();
		}
		//按页模式连续写入
		I2CWrite(addr);
		while(len>0){
			I2CWrite(*buf++);
			len--;
			addr++;
			if((addr&0x7)==0){
				//为8的倍数就翻页
				break;
			}
		}
		I2CStop();
	}
}