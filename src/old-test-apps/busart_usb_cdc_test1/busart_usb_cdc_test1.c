/* File:   busart_usb_cdc_test1.c
   Author: M. P. Hayes, UCECE
   Date:   29 May 2013
   Descr:  This loops a USART to USB CDC using the buffered USART (busart) driver.
           It is useful for communicating with a serial device, such as a bluetooth 
           module.
*/
#include <stdio.h>
#include "busart.h"
#include "usb_cdc.h"
#include "sys.h"
#include "pio.h"
#include "delay.h"

static const usb_cdc_cfg_t usb_cdc_cfg =
{
    /* Non-blocking I/O  */    
    .read_timeout_us = 0,
    .write_timeout_us = 0
};


static const busart_cfg_t busart_cfg =
{
    .channel = 0,               /* USART0 */
    .baud_rate = 115200,
    .tx_size = 64,
    .rx_size = 64
};


int main (void)
{
    char txbuffer[64];
    char rxbuffer[64];
    busart_t busart;
    usb_cdc_t usb_cdc;

    pio_config_set (PA0_PIO, PIO_OUTPUT_LOW);
    pio_config_set (PA1_PIO, PIO_OUTPUT_LOW);

    /* Reset.  */
    pio_config_set (PA27_PIO, PIO_OUTPUT_LOW);
    /* Power.  */
    pio_config_set (PA30_PIO, PIO_OUTPUT_LOW);

    delay_ms (10);

    busart = busart_init (&busart_cfg);
    usb_cdc = usb_cdc_init (&usb_cdc_cfg);
    
    sys_redirect_stdin ((void *)usb_cdc_read, usb_cdc);
    sys_redirect_stdout ((void *)usb_cdc_write, usb_cdc);
    sys_redirect_stderr ((void *)usb_cdc_write, usb_cdc);

    /* Wait until USB configured.  */
    while (! usb_cdc_update ())
        continue;

    pio_config_set (PA0_PIO, PIO_OUTPUT_HIGH);

    delay_ms (5000);

    pio_config_set (PA1_PIO, PIO_OUTPUT_HIGH);

    /* Enable power.  */
    pio_config_set (PA30_PIO, PIO_OUTPUT_HIGH);

    while (1)
    {
        if (usb_cdc_read_ready_p (usb_cdc))
        {
            char ch;

            ch = fgetc (stdin);
            busart_putc (busart, ch);

            /* Echo character.  */
            fputc (ch, stderr);
        }

        if (busart_read_ready_p (busart))
        {
            char ch;

            ch = busart_getc (busart);

            fputc (ch, stderr);
        }
    }
}
