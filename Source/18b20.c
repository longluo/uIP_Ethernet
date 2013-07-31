//对18b20进行预操作步骤：1、复位    2、发送一条ROM指令   3、发送一条RAM指令
//默认12位，最长转换时间750ms

#include <reg52.h>
#include <intrins.h>

/* crystal 22.1184M */

//unsigned char code led_tbl[]={0X3F,0X6,0X5B,0X4F,0X66,0X6D,0X7D,0X7,0X7F,0X6F};	   	//数码管共阳码

code unsigned char decimalH[]={00,06,12,18,25,31,37,43,50,56,62,68,75,81,87,93};	//查表:小数高位

code unsigned char decimalL[]={00,25,50,75,00,25,50,75,00,25,50,75,00,25,50,75};	//查表:小数低位

//unsigned char code scan_tbl[]={0x01,0x02,0x04,0x08};								//数码管扫描码
//unsigned char display_buffer[4]={1,2,3,4};													//显示缓冲

//----------------------------------------------------------------------
#define SCAN_PORT P0	 //扫描口 低有效 P0_0->NUM3	 ~P0_3->NUM0
#define DATA_PORT P2	 //数据口 低点亮
sbit P1_0=P1^0;
#define DQ        P1_0   //定义通信端口
//----------------------------------------------------------------------
#define delay delay18b20
//功能：延时函数
void delay(unsigned int i)
{
	while(i--);
}
//-----------------------------------------------------------------------
//功能：18B20复位函数
 unsigned char DS18B20_RST(void)
{
	unsigned char temp;
	DQ = 1;    	//DQ复位
	delay(16);  	//稍做延时
	DQ = 0;    	//单片机将DQ拉低
	delay(100); 	//精确延时 大于 480us	   精确延时:505us
	DQ = 1;    	//拉高总线	input(RX)			   
	delay(16);	//						   精确延时: 96us
	temp=DQ;
	delay(80);  //						   精确延时: 408us    408+96=504us>480us 满足时序
													   
	//为什么用delay(4);return DQ;时检测到18B20 而转换不对，而加delay(14);return DQ;没检
	//测到18B20，而转换正确  因为回复时间至少480us

	return temp;	//返回1，复位失败     返回0，复位成功
}
//------------------------------------------------------------------------
//功能：读一个字节
unsigned char ReadOneChar(void)
{
	unsigned char i=0;
	unsigned char dat = 0;
	for (i=8;i>0;i--)
	{
		DQ = 0; 		// 拉底总线，读slot开始
		dat>>=1;
		DQ = 1; 		// 释放总线
		if(DQ)
		dat|=0x80;	  	//LSB first
		delay(8);
	}
	return(dat);
}

//功能：写一个字节
WriteOneChar(unsigned char dat)
{
	unsigned char i=0;
	for (i=8; i>0; i--)
	{
		DQ = 0;
		_nop_();
		DQ = dat&0x01;		//LSB first
		delay(10);			//6.6ms
		DQ = 1;
		dat>>=1;
 }

}
//---------------------------------------------------------------------------
//读取温度
unsigned int ReadTemperature(void)
{
	unsigned int  tp=0;
	unsigned char temp_L,temp_H; //高位 和 低位

	DS18B20_RST();	  			//						 步骤 1	
	WriteOneChar(0xCC); 		// 跳过读序号列号的操作	 步骤 2
	WriteOneChar(0x44); 		// 启动温度转换		 	 步骤 3	
	while(DQ==0);				//等待转换完成  DQ=0表示正在转换，DQ=1表示转换完成 12位需要最多750ms

	DS18B20_RST();
	WriteOneChar(0xCC); 		//跳过读序号列号的操作 
	WriteOneChar(0xBE); 		//读取温度寄存器等（共可读9个寄存器） 前两个就是温度

	temp_L = ReadOneChar();		//低8位（先低后高的发过来的）

	temp_H = ReadOneChar(); 	//高八位
	tp|=temp_H;
	tp<<=8;
	tp|=temp_L;
	return tp;
}

unsigned int get_t(void)
{
	unsigned int t,return_t;
	unsigned char temp1,temp2;

	t=ReadTemperature();

	if(t&0x8000)
	{
	//温度为负
	}
	else
	{
	 	temp1=(unsigned char)(t>>4);
		temp2=(unsigned char)(t&0x000f);
		return_t=decimalH[temp2];
		
		temp1%=100;
		return_t|=(temp1<<8);
	}
	return return_t;		
}

	

#undef delay
  