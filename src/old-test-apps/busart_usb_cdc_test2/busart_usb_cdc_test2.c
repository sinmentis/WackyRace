/* File:   busart_usb_cdc_test2.c
   Author: M. P. Hayes, UCECE
   Date:   29 May 2013
   Descr: This loops a USART to USB CDC using the buffered USART
           (busart) driver.  It is useful for communicating with a
           serial device, such as a bluetooth module.  I/O is
           performed using the C library.  See busart_usb_cdc_test3 as
           a comparison.
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


static void
io_init (void)
{
    busart_t busart;
    usb_cdc_t usb_cdc;

    pio_config_set (LED1_PIO, PIO_OUTPUT_LOW);
    pio_config_set (LED2_PIO, PIO_OUTPUT_LOW);

    /* Initialise BUSART driver (this is a buffered USART driver that
       uses interrupts and ring buffers for efficiency). Then,
       register the BUSART driver as a device that can be opened using
       C stdio.  Note, the BUSART driver does not support line
       buffering or handling of carriage-return/newline
       characters.  */
    busart = busart_init (&busart_cfg);
    sys_device_register ("/dev/usart", &busart_file_ops, busart);
    
    /* Initialise USB CDC driver. Then, register the USB CDC driver as
       a device that can be opened using C stdio.  Note, the USB CDC
       driver does not support line buffering or handling of
       carriage-return/newline characters.  */
    usb_cdc = usb_cdc_init (&usb_cdc_cfg);
    sys_device_register ("/dev/usb_cdc", &usb_cdc_file_ops, usb_cdc);
}


int main (void)
{
    FILE *usart_stream;
    FILE *usb_cdc_stream;

    /* Normally, an operating system would do all this sort of stuff 
       when a computer boots.  With a bare-metal system we need to do it
       ourself.  */
    io_init ();

    usb_cdc_stream = fopen ("/dev/usb_cdc", "r+");
    usart_stream = fopen ("/dev/usart", "r+");

    while (1)
    {
        int ch;

        /* These streams are non-blocking and so will normally return EOF.  */
        ch = fgetc (usb_cdc_stream);
        if (ch != EOF)
        {
            /* Echo character.  */
            fputc (ch, usb_cdc_stream);

            /* Forward character.  */
            fputc (ch, usart_stream);
            pio_output_toggle (LED1_PIO);
        }

        ch = fgetc (usart_stream);
        if (ch != EOF)
        {
            /* Echo character.  */
            fputc (ch, usart_stream);

            /* Forward character.  */
            fputc (ch, usb_cdc_stream);
            pio_output_toggle (LED2_PIO);
        }
    }
}
