#ifndef _UART_H_
#define _UART_H_

#include "etherdev.h"
//#include <stdio.h>


#ifdef UART_GLOBALS  
#define UART_EXT
#else
#define	UART_EXT extern
#endif

#define BPS	9600
#define UIP_COM_RX_BUF_SIZE 100

UART_EXT xdata unsigned char uip_combuf[];
UART_EXT xdata unsigned int uip_comwr;
UART_EXT xdata unsigned int uip_comrd;

UART_EXT void uart_init(void);
UART_EXT void puts(unsigned char  *string);
UART_EXT void put(unsigned char ascii);


#endif