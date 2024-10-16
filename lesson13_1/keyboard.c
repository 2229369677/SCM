#include <REG52.H>

sbit KEY_IN_1  = P2^4;
sbit KEY_IN_2  = P2^5;
sbit KEY_IN_3  = P2^6;
sbit KEY_IN_4  = P2^7;
sbit KEY_OUT_1 = P2^3;
sbit KEY_OUT_2 = P2^2;
sbit KEY_OUT_3 = P2^1;
sbit KEY_OUT_4 = P2^0;

//按键映射
unsigned char code KeyCodeMap[4][4] ={
	{'1','2','3',0x26},
	{'4','5','6',0x25},
	{'7','8','9',0x28},
	{'0',0x1B,0x0D,0x27}
};//将数据存储在ROM

unsigned char pdata KeySta[4][4] = {  //按键当前状态
    {1, 1, 1, 1},  {1, 1, 1, 1},  {1, 1, 1, 1},  {1, 1, 1, 1}
};//将数据存储在外部扩展RAM的低256字节空间

extern void KeyAction(unsigned char n);

//按键驱动
void KeyDriver(){
	unsigned char i,j;
	static unsigned char pdata backup[4][4] = {  //按键值备份，保存前一次的值
		{1, 1, 1, 1},  {1, 1, 1, 1},  {1, 1, 1, 1},  {1, 1, 1, 1}
	};
		
	for(i = 0;i<4;i++){
		for(j = 0;j<4;j++){
			if(KeySta[i][j]!=backup[i][j]){//按键发生变化
				if(backup[i][j]!=0){//前一次是高电压
					KeyAction(KeyCodeMap[i][j]);
				}
				backup[i][j] = KeySta[i][j];//更新值
			}
		}
	}
}


void KeyScan(){
	unsigned char i;
	static unsigned char keyout = 0;//矩阵按键扫描输入索引
	static unsigned char keybuf[4][4] = {
		{0xff,0xff,0xff,0xff},
		{0xff,0xff,0xff,0xff},
		{0xff,0xff,0xff,0xff},
		{0xff,0xff,0xff,0xff}
	};
	
	//将第一行4个按键移入缓冲区
	keybuf[keyout][0] = (keybuf[keyout][0]<<1)|KEY_IN_1;
	keybuf[keyout][1] = (keybuf[keyout][1]<<1)|KEY_IN_2;
	keybuf[keyout][2] = (keybuf[keyout][2]<<1)|KEY_IN_3;
	keybuf[keyout][3] = (keybuf[keyout][3]<<1)|KEY_IN_4;
	//消抖后更新按键状态
	for(i = 0;i<4;i++){
		if((keybuf[keyout][i]&0x0f)==0x00){//4位都是0
			KeySta[keyout][i] = 0;
		}
		else if(keybuf[keyout][i]&0x0f==0x0f){
			KeySta[keyout][i] = 1;
		}
	}
	keyout++;
	keyout &= 0x03;
	switch(keyout){
		case 0: KEY_OUT_4 = 1; KEY_OUT_1 = 0; break;
		case 1: KEY_OUT_1 = 1; KEY_OUT_2 = 0; break;
		case 2: KEY_OUT_2 = 1; KEY_OUT_3 = 0; break;
		case 3: KEY_OUT_3 = 1; KEY_OUT_4 = 0; break;
		default: break;
	}
}