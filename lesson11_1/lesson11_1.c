#include <REG52.H>

sbit PIN_RXD = P3^0;
sbit PIN_TXD = P3^1;

bit RxdEnd = 0;//接收结束标识
bit TxdEnd = 0;//发送结束标识
bit RxdOrTxd = 0;//接收or发送标识
unsigned char RxdBuf = 0;//接收数据
unsigned char TxdBuf = 0;//发送数据
void ConfigUART(unsigned int baud);//设置波特率
void StartRXD();//接收
void StartTXD(unsigned char dat);//发送

void main(){
	EA = 1;
	ConfigUART(9600);
	
	while(1){
		while(PIN_RXD);
		StartRXD();
		while(!RxdEnd);
		StartTXD(RxdBuf+1);
		while(!TxdEnd);
	}
}

void ConfigUART(unsigned int baud){
	TMOD &= 0xf0;
	TMOD |= 0x02;//模式2，TL0溢出，自动将TH0重装进TL0；
	TH0 = 256 - (11059200/12)/baud;
}

void StartRXD(){
	TL0 = 256-((256 - TH0)>>1);//在接收信号的中断接收
	ET0 = 1;//打开定时器0中断
	TR0 = 1;//打开定时器0
	RxdEnd = 0;//打开接收
	RxdOrTxd = 0;//接收状态
}

void StartTXD(unsigned char dat){
	TxdBuf = dat;//要发送的数据
	TL0 = TH0;
	ET0 = 1;
	TR0 = 1;
	PIN_TXD = 0;//发0,开始提示标识
	TxdEnd = 0;//打开发送
	RxdOrTxd = 1;//发送状态
}

void InterruptTime0() interrupt 1{
	static unsigned char cnt = 0;
	
	if(RxdOrTxd){//发送
		cnt++;
		if(cnt<=8){
			PIN_TXD = TxdBuf&0x01;//发送低位一位
			TxdBuf>>=1;
		}
		else if(cnt==9){
			PIN_TXD = 1;//发1，结束提示标识
		}
		else{//结束
			cnt = 0;
			TR0 = 0;
			TxdEnd = 1;
		}
	}
	else{//接收
		if(cnt == 0){
			if(!PIN_RXD){//判断是否是干扰
				RxdBuf = 0;
				cnt++;
			}
			else{
				TR0 = 0;
			}
		}
		else if(cnt<=8){
			RxdBuf>>=1;
			if(PIN_RXD){
				RxdBuf |= 0x80;
			}
			cnt++;
		}
		else{
			cnt = 0;
			TR0 = 0;
			if(PIN_RXD){
				RxdEnd = 1;
			}
		}
	}
}