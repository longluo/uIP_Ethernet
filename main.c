#include "main.h"

void main(void)
{ 
    u8_t xdata i, arptimer;

    /* Initialise the uIP TCP/IP stack. */
    uip_init();

    /* Initialise the app. */
//    httpd_init();

    /* Initialise the device driver. */ 
    etherdev_init();

    /* Initialise the ARP cache. */
    uip_arp_init();
	
	/*Initialise uart 9600bps */
	uart_init(); 
	
	/*Initialise listen port */
	listen_init();  

	DS18B20_RST();

	EA=1;


    arptimer = 0;

    while(1)
    {
        uip_len = etherdev_read();

        if(uip_len == 0)
        {
            for(i = 0; i < UIP_CONNS; i++)
            {
                uip_periodic(i);
                /* If the above function invocation resulted in data that
                   should be sent out on the network, the global variable
                   uip_len is set to a value > 0. */
                if(uip_len > 0)
                {
                    uip_arp_out();
                    etherdev_send();
                }
            }

            /* Call the ARP timer function every 10 seconds. */
            if(++arptimer == 20)
            {	
	            uip_arp_timer();
	            arptimer = 0;
            }
        }
        else /* (uip_len != 0) Process incoming */
        {
            if(BUF->type == htons(UIP_ETHTYPE_IP))
            {
	            uip_arp_ipin();
	            uip_input();
	            /* If the above function invocation resulted in data that
	               should be sent out on the network, the global variable
	               uip_len is set to a value > 0. */
	            if(uip_len > 0)
                {
	                uip_arp_out();
	                etherdev_send();
	            }
            }
            else if(BUF->type == htons(UIP_ETHTYPE_ARP))
            {
	            uip_arp_arpin();
	            /* If the above function invocation resulted in data that
	               should be sent out on the network, the global variable
	               uip_len is set to a value > 0. */	
	            if(uip_len > 0)
                {	
	                etherdev_send();
	            }
            }
        }	
    }

    return;
}
