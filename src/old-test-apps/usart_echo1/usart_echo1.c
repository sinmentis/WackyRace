/* File:   usart_echo1.c
   Author: M. P. Hayes, UCECE
   Date:   27 May 2015
   Descr: 
*/
#include <stdio.h>
#include "usart.h"

static const usart_cfg_t usart_cfg =
{
    .channel = 0,               /* USART0 */
    .baud_rate = 115200,
    /* With blocking I/O, the function does not return until all the
       I/O has been transferred.  This is not very efficient since
       there is a lot of busy-wait polling on a non-multitasked
       system.  With non-blocking I/O it is necessary to check
       function returns in case the data was not transferred.  */
    .read_timeout_us = ~0,
    .write_timeout_us = ~0
};

int main (void)
{
    usart_t usart;

    usart = usart_init (&usart_cfg);
    
    usart_puts (usart, "Hello world\n");
    
    while (1)
    {
        int ch;

	ch = usart_getc (usart);
        if (ch != EOF)
            usart_putc (usart, ch);
    }
}
