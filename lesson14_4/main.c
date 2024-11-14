//用分页模式
#include <REG52.H>

extern void InitLcd1602();
extern void LcdShowStr(unsigned char x, unsigned char y, unsigned char *str);
extern void E2Read(unsigned char *buf, unsigned char addr, unsigned char len);
extern void E2Write(unsigned char *buf, unsigned char addr, unsigned char len);
void MemToStr(unsigned char *str, unsigned char *src, unsigned char len);

void main(){
	unsigned char i;
	unsigned char buf[5];
	unsigned char str[20];
	
	InitLcd1602();
	E2Read(buf,0x90,sizeof(buf));//从E2中读取一段数据
	MemToStr(str,buf,sizeof(buf));//转换为16进制字符串
	LcdShowStr(0,0,str);
	for(i = 0;i<sizeof(buf);i++){
		buf[i] = buf[i]+i;
	}
	E2Write(buf,0x90,sizeof(buf));//在写回去
	
	while(1);
}

void MemToStr(unsigned char *str, unsigned char *src, unsigned char len){
	unsigned char tmp;
	while(len--){
		tmp = *src>>4;
		if(tmp<=9){
			*str++ = tmp+'0';
		}else{
			*str++ = tmp-10+'A';
		}
		tmp = *src & 0x0f;
		if(tmp<=9){
			*str++ = tmp+'0';
		}else{
			*str++ = tmp-10+'A';
		}
		*str++=' ';
		src++;
	}
	*str = '\0';
}