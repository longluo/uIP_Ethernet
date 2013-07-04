#include <absacc.h>
#include "etherdev.h"


#define ETH_CPU_CLOCK      ETH_CPU_XTAL / 12    // 8051 clock rate (X1 mode)

// Delay routine timing parameters
//#define ETH_DELAY_CONST    9.114584e-5          // Delay routine constant
//#define ETH_DELAY_MULTPLR  (unsigned char)(ETH_DELAY_CONST * ETH_CPU_CLOCK)

// X1 CPU mode timing parameters
#define ETH_T0_CLOCK             ETH_CPU_XTAL / 12 // Timer 0 mode 1 clock rate
//#define ETH_T0_INT_RATE          24                // Timer 0 intrupt rate (Hz)
#define ETH_T0_INT_RATE          48

#define ETH_T0_RELOAD            65536 - (ETH_T0_CLOCK / ETH_T0_INT_RATE)

#define ETH_MIN_PACKET_LEN 0x3C

// Packet transmit & receive buffer configuration
//#define ETH_TX_PAGE_START  0x40    // 0x4000 Tx buffer is  6 * 256 = 1536 bytes
//#define ETH_RX_PAGE_START  0x46    // 0x4600 Rx buffer is 26 * 256 = 6656 bytes
//#define ETH_RX_PAGE_STOP   0x60    // 0x6000

//#define ETH_ADDR_PORT_MASK 0x1F                 // 00011111y
//#define ETH_DATA_PORT_MASK 0xFF                 // 11111111y

//#define ETH_MIN_PACKET_LEN 0x3C

static unsigned char tick_count = 0;
static bit Tx_Buff_Sel;

#define     RTL8019AS_BASE_ADDRESS  0x8000
//*********** RTL8019's address deifne***************
#define     RED_ADDR0   RTL8019AS_BASE_ADDRESS+0x0000
#define     RED_ADDR1   RTL8019AS_BASE_ADDRESS+0x0001
#define     RED_ADDR2   RTL8019AS_BASE_ADDRESS+0x0002
#define     RED_ADDR3   RTL8019AS_BASE_ADDRESS+0x0003
#define     RED_ADDR4   RTL8019AS_BASE_ADDRESS+0x0004
#define     RED_ADDR5   RTL8019AS_BASE_ADDRESS+0x0005
#define     RED_ADDR6   RTL8019AS_BASE_ADDRESS+0x0006
#define     RED_ADDR7   RTL8019AS_BASE_ADDRESS+0x0007
#define     RED_ADDR8   RTL8019AS_BASE_ADDRESS+0x0008
#define     RED_ADDR9   RTL8019AS_BASE_ADDRESS+0x0009
#define     RED_ADDRa   RTL8019AS_BASE_ADDRESS+0x000a
#define     RED_ADDRb   RTL8019AS_BASE_ADDRESS+0x000b
#define     RED_ADDRc   RTL8019AS_BASE_ADDRESS+0x000c
#define     RED_ADDRd   RTL8019AS_BASE_ADDRESS+0x000d
#define     RED_ADDRe   RTL8019AS_BASE_ADDRESS+0x000e
#define     RED_ADDRf   RTL8019AS_BASE_ADDRESS+0x000f
#define     RED_ADDR10  RTL8019AS_BASE_ADDRESS+0x0010
#define     RED_ADDR18  RTL8019AS_BASE_ADDRESS+0x0018

#define	 	Reg00   XBYTE[RED_ADDR0]   
#define 	Reg01   XBYTE[RED_ADDR1]
#define 	Reg02   XBYTE[RED_ADDR2]
#define 	Reg03   XBYTE[RED_ADDR3]
#define 	Reg04   XBYTE[RED_ADDR4]
#define 	Reg05   XBYTE[RED_ADDR5]
#define 	Reg06   XBYTE[RED_ADDR6]
#define 	Reg07   XBYTE[RED_ADDR7]
#define 	Reg08   XBYTE[RED_ADDR8]
#define 	Reg09   XBYTE[RED_ADDR9]
#define 	Reg0a   XBYTE[RED_ADDRa]
#define 	Reg0b   XBYTE[RED_ADDRb]
#define 	Reg0c   XBYTE[RED_ADDRc]
#define 	Reg0d   XBYTE[RED_ADDRd]
#define 	Reg0e   XBYTE[RED_ADDRe]
#define 	Reg0f   XBYTE[RED_ADDRf]
#define 	Reg10   XBYTE[RED_ADDR10]
#define 	Reg18   XBYTE[RED_ADDR18]


static unsigned int etherdev_poll(void);


/**********************************************************************
**函数原型：  void Delay(unsigned int8 t)	 内部调用
**入口参数: 	unsigned int8 t:延时常数   
**说    明：	微秒级延时程序 
************************************************************************/
void Delay(unsigned char t)
{	while(t--);
}

/**********************************************************************
**函数原型：    void Delay_Ms(unsigned int16 t1)	晶振24.576M
**入口参数: 	unsigned int8 t1:毫秒级延时，延时约为t1ms
**说    明：	毫秒级延时程序 
************************************************************************/ 
void Delay_Ms(unsigned int t1)
{	while(t1--)
	{
	#if ETH_CPU_XTAL==11059200 
		Delay(151);	   	//11059200Hz
	#else			  	//ETH_CPU_XTAL=22118400Hz
		Delay(150);
		Delay(152);
	#endif
	}		
}

/**********************************************************************
**函数原型：  void  page(unsigned char pagenumber)
**入口参数:		unsigned char	pagenumber: 要切换的页    
**说    明：	选择页,可选择0,1,2三页,第四页ne000兼容芯片保留 
************************************************************************/
void page(unsigned char pagenumber)
{	unsigned char data temp;
	temp=Reg00;//command register
	//temp=temp&0x3f;
	temp=temp&0x3B ;//注意txp位不能要
	pagenumber=pagenumber <<6;
	temp=temp | pagenumber;
	Reg00=temp;
}

void Set_Mac()
{   page(1);
    Reg01 = UIP_ETHADDR0;
    Reg02 = UIP_ETHADDR1;
    Reg03 = UIP_ETHADDR2;
    Reg04 = UIP_ETHADDR3;
    Reg05 = UIP_ETHADDR4;
    Reg06 = UIP_ETHADDR5;
    page(0);
}

/**********************************************************************
**函数原型:     void 	InitNic()            
**说    明:对芯片的工作寄存器进行设置,各个寄存器的用法可参考文档和
**				 络芯片的数据手册
************************************************************************/
void etherdev_init(void)
{	unsigned char temp;
	ETH_RST_PIN=0;             //reset
	Delay_Ms(60);
	ETH_RST_PIN=1;
	Delay_Ms(10);
	ETH_RST_PIN=0; 
	Delay_Ms(100);

	//use reset port to reset the ic
	temp=Reg18;
	Reg18=temp;

	if(Reg04 & 0x80)	   //ISR.RST
	{//	puts("reset fail\r\n");
		return;
	}
	Reg00=0x21;   //使芯片处于停止模式,这时进行寄存器设置
	Delay_Ms(100); //延时10毫秒,确保芯片进入停止模式
	
	page(0);
	Reg0a=0x00;   //清rbcr0
	Reg0b=0x00;   //清rbcr1
	Reg0c= 0xe0;  //RCR,监视模式,不接收数据包
	Reg0d= 0xe2;  //TCR,loop back模式
	
	Reg01=0x4c;

//	Reg02=0x80;
	Reg02=0x60;
	Reg03=0x4c;
	Reg04=0x40;  //TPSR,发送起始页寄存器
	Reg07=0xff;  //清除所有中断标志位，中断状态寄存器
	Reg0f=0x00;  //中断屏蔽寄存器清0，禁止中断
	
	Reg0e=0xc8;  // 数据配置寄存器，8位dma方式
	page(1);
	Reg07=0x4d;
	Reg08=0x00;
	Reg09=0x00;
	Reg0a=0x00;
	Reg0b=0x00;
	Reg0c=0x00;
	Reg0d=0x00;
	Reg0e=0x00;
	Reg0f=0x00;
	Reg00=0x22;  //这时让芯片开始工作   
	Set_Mac();  //将芯片物理地址写入到MAR寄存器
  
	page(0);
	//Reg0c=0xcc;  //将芯片设置成正常模式,跟外部网络连接
	Reg0c=0xc4;	   //接收广播，不接收多播
	Reg0d=0xe0;
	
	Reg00=0x22;  //启动芯片开始工作
	Reg07=0xff;  //清除所有中断标志位
    
	TR0  = 0;          
	// Set timer/ counter 0 as mode 1 16 bit timer.      
	TMOD &= 0xF0;
	TMOD |= 0x01;
	// Preload for 24Hz periodic interrupt.    
	TH0 = ETH_T0_RELOAD >> 8; 
	TL0 = ETH_T0_RELOAD;
	// Restart timer/ counter 0 running.
	TR0 = 1;
	// Enable timer/ counter 0 overflow interrupt.            
	ET0 = 1;
	// Enable global interrupt.
	EA = 1;
}

/**********************************************************************
**函数原型：    void 		Send_Packet(union Netcard *TxdNetBuff,unsigned int length)
**入口参数:		union Netcard *TxdNetBuff	:指向发送缓冲区
**              unsigned int length					:发送数据包的长度
**出口参数:		无
**返 回 值：	无              
**说    明：	发送数据包,以太网底层驱动程序,所有的数据发送都要通过该程序
************************************************************************/
void etherdev_send(void)
{	unsigned int i;
	unsigned char *ptr;
	ptr = uip_buf;
	
	page(0);					//切换至第0页
	Tx_Buff_Sel=!Tx_Buff_Sel;	//设置两个发缓区,提高发送效率
	if(Tx_Buff_Sel)
		{Reg09=0x40 ;}			//RSAR1 设置发送页地址
	else
		{Reg09=0x46 ;}			//RSAR1设置发送页地址	    
	Reg08=0x00; 					//RSAR0 read page address low
	Reg0b=uip_len>>8;		//RBCR1 read count high
	Reg0a=uip_len&0xff;	//RBCR0 read count low;
	
	Reg00=0x12;			//write dma, page0
	
	// DMA transfer packet from uip_buf & uip_appdata to RTL8019AS local
	// transmit buffer memory.
	for(i = 0; i < uip_len; i++)
	{   if(i == 40 + UIP_LLH_LEN) 
	  {
	      ptr = (unsigned char *)uip_appdata;
	  }
	  Reg10=*ptr++;
	}
	while(Reg00 & 0x40);	//wait for the last transmit complete
/*	
	for(i=0;i<6;i++)	//最多重发6次
	{
		for(ii=0;ii<1000;ii++)
	    	{//检查CR寄存器的txp位是否为低,为1说明正在发送,为0说明发完或出错放弃
	        if((Reg00&0x04)==0)  
	        	{ break; }
	        }
		if((Reg04&0x01)!=0)//表示发送成功,判断发送状态寄存器TSR.PTX，决定是否出错
			{break;};
		Reg00=0x3e;		 //?
	}
*/	
	Reg07=0xff;
	if(Tx_Buff_Sel)
	{
	  	Reg04=0x40;   //TPSR	txd packet start;
	}
	else
	{
	   	Reg04=0x46;  //TPSR		txd packet start;
	}
	
	// Ethernet packets must be > 60 bytes, otherwise are rejected as runts.
	if(uip_len < ETH_MIN_PACKET_LEN)
	{	uip_len = ETH_MIN_PACKET_LEN;
	}
	
	Reg06=uip_len>>8;  //high byte counter
	Reg05=uip_len&0xff;//low byte counter
	
	Reg07=0xff;
	Reg00=0x3e;       //to sendpacket;
	while(Reg04&0x01==0);
}

/*  赏屯屯屯屯屯屯屯屯屯屯屯屯屯屯屯屯屯屯屯屯屯屯屯屯屯屯屯屯屯屯屯屯屯屯?
    ?                                                                    ?
    ?                          etherdev_read()                           ?
    ?                                                                    ?
    ?This function will read an entire IP packet into the uip_buf.       ?
    ?If it must wait for more than 0.5 seconds, it will return with      ?
    ?the return value 0. Otherwise, when a full packet has been read     ?
    ?into the uip_buf buffer, the length of the packet is returned.      ?
    ?                                                                    ?
    韧屯屯屯屯屯屯屯屯屯屯屯屯屯屯屯屯屯屯屯屯屯屯屯屯屯屯屯屯屯屯屯屯屯屯? */
unsigned int etherdev_read(void)
{    
    unsigned int bytes_read;

    /* tick_count threshold should be 12 for 0.5 sec bail-out
       One second (24) worked better for me, but socket recycling
       is then slower. I set UIP_TIME_WAIT_TIMEOUT 60 in uipopt.h
       to counter this. Retransmission timing etc. is affected also. */
    while ((!(bytes_read = etherdev_poll())) && (tick_count < (ETH_T0_INT_RATE/2))) continue;

    tick_count = 0;

    return bytes_read;
}

/*****************************************************************************
*								etherdev_poll()                           
*			Poll the RTL8019AS ethernet device for an available packet.         
******************************************************************************/
static unsigned int etherdev_poll(void)
{
    unsigned char i;
	unsigned int ii,_len;
    unsigned char bnry,curr;
	unsigned char xdata rxdbuf[18];
  
    page(0);
    Reg07=0xFF;
    bnry=Reg03;               //bnry page have read 读页指针
    page(1);
    curr=Reg07;               //curr writepoint 8019写页指针
    page(0);
    if(curr==0)
        return 0;             //读的过程出错 
    bnry=bnry++;

//    if(bnry>0x7F) bnry=0x4C;
	if(bnry>0x5F) bnry=0x4C;
    if(bnry!=curr){           //此时表示有新的数据包在缓冲区里
 //读取一包的前18个字节:4字节的8019头部,6字节目的地址,6字节原地址,2字节协议
 //在任何操作都最好返回page0
        page(0);
        Reg09=bnry;           //read page address high
        Reg08=0x00;           //read page address low
        Reg0b=0x00;           //read count high
        Reg0a=18;             //read count low;
        Reg00=0x0A;           //read dma
        for(i=0;i<18;i++)
            rxdbuf[i]=Reg10;

        i=rxdbuf[3];     //将长度字段的高低字节掉转
        rxdbuf[3]=rxdbuf[2];
        rxdbuf[2]=i;    
        _len=rxdbuf[2]*256+rxdbuf[3]-4; //去掉4个字节的CRC

        //表示读入的数据包有效
      //  if(((rxdbuf[0]&0x01)==0)||(rxdbuf[1]>0x7F)||(rxdbuf[1]<0x4C)||(rxdbuf[2]>0x06)){
		if(((rxdbuf[0]&0x01)==0)||(rxdbuf[1]>0x5F)||(rxdbuf[1]<0x4C)||(rxdbuf[2]>0x06)){
            //接收状态错误,或者next_page_start错误或者长度错误,将丢弃所有数据包
            page(1);
            curr=Reg07;       //page1
            page(0);          //切换回page0
            bnry=curr-1;
            //if(bnry<0x4C) bnry=0x7F;
			if(bnry<0x4C) bnry=0x5F;
            Reg03=bnry;       //write to bnry     
            return 0;
        }
        else
		{//表示数据包是完好的.读取剩下的数据
            if((rxdbuf[16]*256+rxdbuf[17]==0x0800)||(rxdbuf[16]*256+rxdbuf[17]==0x0806))
			{
            //协议为IP或ARP才接收       
                Reg09=bnry;   //RSAR1	read page address high
                Reg08=4;      //RSAR0	不要4byte的8019as头部

                Reg0b=_len>>8;     	//RBCR1	read count high
                Reg0a=_len&0xFF;   	//RBCR0	read count low;
                Reg00=0x0A;   		//re/ad dma

                for(ii=0;ii<_len;ii++)
                    *(uip_buf+ii)=Reg10;

                //================终止DMA操作
                Reg0b=0x00;			//read count high   
                Reg0a=0x00;			//read count low;
                Reg00=0x22;			//结束或放弃DMA
                //============================
            }
            bnry=rxdbuf[1]-1;//next page start-1

            //if(bnry<0x4C) bnry=0x7F;
			if(bnry<0x4C) bnry=0x5F;
            Reg03=bnry;       //write to bnry 
      
            return _len;         //返回包长
        }
    }
    return 0;
}

static void etherdev_timer0_isr(void) interrupt 1 using 1
{
    // Reload timer/ counter 0 for 24Hz periodic interrupt.   
    TH0 = ETH_T0_RELOAD >> 8;
    TL0 = ETH_T0_RELOAD;

    // Increment 24ths of a second counter.
    tick_count++;

    return;
}