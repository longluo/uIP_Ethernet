#define UART_GLOBALS

#include "uart.h"

xdata unsigned char uip_combuf[UIP_COM_RX_BUF_SIZE];
xdata unsigned int uip_comwr=0;
xdata unsigned int uip_comrd=0;
//xdata unsigned char uip_combufept=1;	//接收buffer为空标志

/**********************************************************************
**函数原型：    void Uart_Init(void)
**参数说明：	无
**返 回 值：	无      
**说    明：	串口初始化，采用T2，时钟和波特率在net_cfg.h中定义
************************************************************************/
void uart_init(void)
{
// 	RCAP2H=(65536-FOSC/16/BPS)/256;		  //6-clock mode
//	RCAP2L=(65536-FOSC/16/BPS)%256;
	RCAP2H=(65536-ETH_CPU_XTAL/32/BPS)/256;		 //12-clock mode 
	RCAP2L=(65536-ETH_CPU_XTAL/32/BPS)%256;
	SCON = 0x50;	                      //串口工作方式1
	T2CON = 0x34;	                   
//	TI=1;
	PS=1;	//高中断优先级
	ES=1;
}

/**********************************************************************
**函数原型：    void  	Printf_Char(uchar ascii)
**参数说明：	ascii：	发送到串口的ASCII
**返 回 值：	无      
**说    明：	往串口发送一个字符
************************************************************************/
void put(unsigned char ascii)
{	ES=0;
	SBUF=ascii;
	while(!TI);
	TI=0;
	ES=1;
}
/**********************************************************************
**函数原型：    void  		Printf_String(uchar code *string)
**参数说明：	string：	字符串的起始地址
**返 回 值：	无      
**说    明：	向串口发送字符串，碰到0x00结束
************************************************************************/
void puts(unsigned char  *string)
{	while(*string!=0)
	{	put(*string++);
	}
}


/*******************************************************************
串口ISR，执行一些命令
********************************************************************/
void serial(void) interrupt 4
{	if(TI) 
	{	TI=0;} 

	if (RI)
	{	RI=0;
		uip_combuf[uip_comwr++]=SBUF;
		if(uip_comwr==UIP_COM_RX_BUF_SIZE) uip_comwr=0;	
	}
	 
}

