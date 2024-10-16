#include <REG52.H>
//计算器

unsigned char T0RH = 0;
unsigned char T0RL = 0;
unsigned char step = 0;//操作步骤
unsigned char oprt = 0;//运算类型
signed long num1 = 0;
signed long num2 = 0;
signed long result = 0;//结果

extern void InitLcd1602();
extern void LcdShowStr(unsigned char x,unsigned char y,unsigned char*str);
extern void KeyDriver();
extern void LcdFullClear();
extern void KeyScan();
extern void LcdAreaClear(unsigned char x, unsigned char y, unsigned char len);
void ConfigTimer0(unsigned int ms);

void main(){
	EA = 1;//开总中断
	ConfigTimer0(1);//T0定时1ms
	InitLcd1602();//初始化液晶
	LcdShowStr(15,1,"0");
	
	while(1){
		KeyDriver();//按键驱动
	}
}

void ConfigTimer0(unsigned int ms){
	unsigned int temp;
	temp = 11059200/12;//计数频率
	temp = temp*ms/1000;
	temp = 65536-temp;
	T0RH = (unsigned char)(temp>>8);
	T0RL = (unsigned char)temp;
	TMOD &= 0xf0;
	TMOD |= 0x01;
	TH0= T0RH;
	TL0 = T0RL;
	ET0= 1;//启动T0中断
	TR0 = 1;//启动T0
}

//计算器复位，清零变量，清除屏幕显示
void Reset(){
	num1 = 0;
	num2 = 0;
	step = 0;
	LcdFullClear();//屏幕清除
}

//long转换成字符串
unsigned char LongToString(unsigned char *str,signed long dat){
	signed char i = 0;
	unsigned char len = 0;
	unsigned char buf[12];
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


//按键动作函数，n-键盘输入的值
void NumKeyAction(unsigned char n){
	unsigned char len;
	unsigned char str[12];
	
	if(step>1){//计算完成
		Reset();
	}
	if(step == 0){//输入第一操作数
		num1 = num1*10+n;
		len = LongToString(str,num2);//新数值转换为字符串，返回长度
		LcdShowStr(16-len,1,str);//显示到第二行上
	}
	else{
		num2 = num2*10+n;
		len = LongToString(str,num2);
		LcdShowStr(16-len,1,str);
	}
}

void ShowOprt(unsigned char y, unsigned char type)
{
    switch (type)
    {
        case 0: LcdShowStr(0, y, "+"); break;  
        case 1: LcdShowStr(0, y, "-"); break;  
        case 2: LcdShowStr(0, y, "*"); break;  
        case 3: LcdShowStr(0, y, "/"); break;  
        default: break;
    }
}

void OprtKeyAction(unsigned char type){
	
	unsigned char len;
	unsigned char str[12];
	if(step==0){
		len = LongToString(str,num1);//第一操作数转换成字符串
		LcdAreaClear(0,0,16-len);//清除第一行数字
		LcdShowStr(16-len, 0, str);
		ShowOprt(1, type);         //显示操作符
		LcdAreaClear(1, 1, 14);    
		LcdShowStr(15, 1, "0");    
		oprt = type;               
		step = 1;
		
	}
}

//计算结果
void GetResult()
{
    unsigned char len;
    unsigned char str[12];
    
    if (step == 1) 
    {
        step = 2;
        switch (oprt)
        {
            case 0: result = num1 + num2; break;
            case 1: result = num1 - num2; break;
            case 2: result = num1 * num2; break;
            case 3: result = num1 / num2; break;
            default: break;
        }
        len = LongToString(str, num2);
        ShowOprt(0, oprt);
        LcdAreaClear(1, 0, 16-1-len);
        LcdShowStr(16-len, 0, str);
        len = LongToString(str, result);
        LcdShowStr(0, 1, "=");
        LcdAreaClear(1, 1, 16-1-len);
        LcdShowStr(16-len, 1, str);
    }
}

void KeyAction(unsigned char keycode){
	if  ((keycode>='0') && (keycode<='9'))  
    {
        NumKeyAction(keycode - '0');
    }
    else if (keycode == 0x26)
    {
        OprtKeyAction(0);
    }
    else if (keycode == 0x28)
    {
        OprtKeyAction(1);
    }
    else if (keycode == 0x25)
    {
        OprtKeyAction(2);
    }
    else if (keycode == 0x27)
    {
        OprtKeyAction(3);
    }
    else if (keycode == 0x0D)
    {
        GetResult();
    }
    else if (keycode == 0x1B)
    {
        Reset();
        LcdShowStr(15, 1, "0");
    }
}

void InterruptTimer0() interrupt 1{
	TH0 = T0RH;
	TL0 = T0RL;
	KeyScan();
}