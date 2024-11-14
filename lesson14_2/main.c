//用单字节读写模式访问EEPEOM，每次+1后写回

#include <REG52.H>

extern InitLcd1602();
extern LcdShowStr(unsigned char x,unsigned char y,unsigned char * str);
extern I2CStart();
extern I2CStop();
extern unsigned char I2CReadNAK();
extern bit I2CWrite(unsigned char dat);
unsigned char E2ReadByte(unsigned char addr);
void E2WriteByte(unsigned char addr,unsigned char dat);


void main(){
	unsigned char dat;
	unsigned char str[10];
	InitLcd1602();
	dat = E2ReadByte(0x02);//读取指定地址上的一个字节
	str[0] = (dat/100)+'0';
	str[1] = (dat/10%10)+'0';
	str[2] = (dat%10)+'0';
	str[3] = '\0';
	LcdShowStr(0,0,str);
	dat++;
	E2WriteByte(0x02,dat);
	
	while(1);
}

unsigned char E2ReadByte(unsigned char addr){
	unsigned char dat;
	
	I2CStart();
	I2CWrite(0x50<<1);//寻址器件，后续为写操作
	I2CWrite(addr);//写入存储地址
	I2CStart();//发送重复启动信号
	I2CWrite((0x50<<1)|0x01);//后续为读操作
	dat = I2CReadNAK();//读取一个字节数据，给EEPROM 0
	I2CStop();
	
	return dat;
}

void E2WriteByte(unsigned char addr,unsigned char dat){
	I2CStart();
	I2CWrite(0x50<<1);
	I2CWrite(addr);
	I2CWrite(dat);
	I2CStop();
}