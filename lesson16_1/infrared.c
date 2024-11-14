#include <REG52.H>

sbit IR_INPUT = P3^3;

bit irflag = 0;//红外接收标志
unsigned char ircode[4];//红外代码接收缓冲区

void InitInfrared(){
	IR_INPUT = 1;//确保被释放
	TMOD &= 0x0f;
	TMOD |= 0x10;
	TR1 = 0;
	ET1 = 0;//禁用T1中断
	IT1 = 1;//负边沿触发,下降沿
	EX1 = 1;
}


//获取当前高电平的持续时间
unsigned int GetHighTime(){
	TH1 = 0;
	TL1 = 0;
	TR1 = 1;//启动T1计数
	while(IR_INPUT){
		if(TH1>=0x40){//防止错误信号，造成程序假死
			break;
		}
	}
	TR1 = 0;
	
	return (TH1*256 + TL1);
}

unsigned int GetLowTime()
{
    TH1 = 0;
    TL1 = 0;
    TR1 = 1;
    while (!IR_INPUT){
        if (TH1 >= 0x40){         
            break;
        }
    }
    TR1 = 0;

    return (TH1*256 + TL1);  
}

void EXINT1_ISR() interrupt 2{
	unsigned char i,j;
	unsigned char byt;
	unsigned int time;
	
	//引导码
	time = GetLowTime;
	if((time<7833)||(time>8755)){
		IE1 = 0;//清零INT1中断标志
		return;
	}
	
	time = GetHighTime();
	if ((time<3686) || (time>4608))
	{                              
	    IE1 = 0;
	    return;
	}
	
	//接收4个字节,用户码，用户反码，键码，键码反码
	
	for(i = 0;i<4;i++){
		//8个bit
		for(j = 0;j<8;j++){
			time = GetLowTime();
			if ((time<313) || (time>718))
			{                             
			    IE1 = 0;
			    return;
			}

			time = GetHighTime();
			if ((time>313) && (time<718)) 
			{   
			    byt >>= 1;
			}
			else if ((time>1345) && (time<1751))
			{                                   
			    byt >>= 1;  
			    byt |= 0x80; 
			}
			else
			{
			    IE1 = 0;
			    return;
			}
		}
		ircode[i] = byt;
	}
	irflag = 1;
	IE1 = 0;
}
