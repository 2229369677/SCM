#include <REG52.H>

bit flagFrame = 0;//帧接收完成标志,即接收到了一帧新数据
bit flagTxd = 0;//单字节发送完成标志，用来替代TXD中断标志位
unsigned char cntRxd = 0;  
unsigned char pdata bufRxd[64]; 

extern void UartAction(unsigned char *buf, unsigned char len);

void ConfigUART(unsigned int baud){
	SCON = 0x50;//模式1
	TMOD &= 0x0f;
	TMOD |= 0x20;
	TH1 = 256-(11059200/12/32)/baud;
	TL1 = TH1;
	ET1 = 0;//禁用T1中断
	ES = 1;//串口中断
	TR1 = 1;//启动T1
}

//串口数据写入
void UartWrite(unsigned char *buf,unsigned char len){
	while(len--){
		flagTxd = 0;//
		SBUF =*buf++;
		while(!flagTxd);//发送完毕
	}
}

//串口读取命令函数
unsigned char UartRead(unsigned char *buf, unsigned char len){
	unsigned char i;
	
	if(len>cntRxd){
		len = cntRxd;
	}
	for(i = 0;i<len;i++){
		*buf++ = bufRxd[i];
	}
	cntRxd = 0;
	
	return len;
}

//串口驱动
void UartDriver(){
	unsigned  char len;
	unsigned char pdata buf[40];
	
	//读取命令,进行相应操作
	if(flagFrame){
		flagFrame = 0;
		len  = UartRead(buf,sizeof(buf));//将接收到的命令读取到缓冲区中
		UartAction(buf,len);//进行相应操作
	}
}

//要监控什么？？？
//判断帧是否结束,30ms空闲时间-结束
void UartRxMonitor(unsigned char ms){
	static unsigned char cntbkp = 0;
	static unsigned char idletmr = 0;
	
	if(cntRxd>0){
		if(cntbkp!=cntRxd){
			cntbkp = cntRxd;
			idletmr = 0;
		}
		else{
			if(idletmr<30){
				idletmr+=ms;//到达30ms，一帧接收完毕
				if(idletmr>=30){
					flagFrame = 1;
				}
			}
		}
	}
	else{
		cntbkp = 0;
	}
}

//串口中断
void InterruptUART() interrupt 4{
	if(RI){
		RI = 0;
		if(cntRxd<sizeof(bufRxd)){
			bufRxd[cntRxd++] = SBUF;//存入接收缓冲区
		}
	}
	if(TI){
		TI = 0;
		flagTxd = 1;//字节发送完成标志
	}
}