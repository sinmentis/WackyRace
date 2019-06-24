/* File:   uart_echo1.c
   Author: M. P. Hayes, UCECE
   Date:   27 May 2015
   Descr: 
*/
#include <stdio.h>
#include "uart.h"

static const uart_cfg_t uart_cfg =
{
    .channel = 0,               /* UART0 */
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
    uart_t uart;

    uart = uart_init (&uart_cfg);
    
    uart_puts (uart, "Hello world\n");
    
    while (1)
    {
        int ch;

	ch = uart_getc (uart);
        if (ch != EOF)
            uart_putc (uart, ch);
    }
}
