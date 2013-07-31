#ifndef MAIN_H
#define MAIN_H

#include "uip.h"
#include "uip_arp.h"
#include "etherdev.h"
#include "uart.h"
#include "app.h"

#define BUF ((struct uip_eth_hdr *)&uip_buf[0])

extern unsigned char DS18B20_RST(void);


#ifndef NULL
#define NULL (void *)0
#endif /* NULL */

#endif /* MAIN_H */
