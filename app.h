#ifndef __app_h__
#define __app_h__
/*
#ifdef APP_GLOBALS  
#define APP_EXT
#else
#define	APP_EXT extern
#endif
*/
#include "uip.h"
#include "uart.h"

struct httpd_state {
  u16_t count;
  char *dataptr;
};

extern void listen_init(void);
extern void appcall(void);
extern unsigned int get_t(void);

#ifndef UIP_APPCALL
#define UIP_APPCALL appcall
#endif

#endif