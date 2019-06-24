/* File:   busart_echo1.c
   Author: M. P. Hayes, UCECE
   Date:   29 May 2013
   Descr: 
*/
#include <stdio.h>
#include "busart.h"

static const busart_cfg_t busart_cfg =
{
    .channel = 0,               /* USART0  */
    .baud_rate = 115200,        /* Baud rate  */
    .tx_size = 64,              /* Transmit ring buffer size  */
    .rx_size = 64,              /* Receive ring buffer size  */
    /* Blocking I/O  */
    .read_timeout_us = ~0,
    .write_timeout_us = ~0
};


int main (void)
{
    busart_t busart;

    busart = busart_init (&busart_cfg);
    
    busart_puts (busart, "Hello world\n");
    
    while (1)
    {
        char ch;

        /* NB, this blocks!   */
	ch = busart_getc (busart);

        busart_putc (busart, ch);
    }
}
