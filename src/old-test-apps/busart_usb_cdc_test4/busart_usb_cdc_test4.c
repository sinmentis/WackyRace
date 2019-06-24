/* File:   busart_usb_cdc_test4.c
   Author: M. P. Hayes, UCECE
   Date:   29 May 2013

   Descr: This loops a USART to USB CDC using the buffered USART
           (busart) driver.  It is useful for communicating with a
           serial device, such as a bluetooth module.  I/O is
           performed using the C library with fopencookie.  It uses a
           TTY driver to provide line buffering and
           carriage-return/newline handling.  See busart_usb_cdc_test2
           and busart_usb_cdc_test3 as a comparison.
*/
#include <stdio.h>
#include "busart.h"
#include "usb_cdc.h"
#include "sys.h"
#include "pio.h"
#include "tty.h"
#include "delay.h"


static const usb_cdc_cfg_t usb_cdc_cfg =
{
    /* Blocking I/O  */    
    .read_timeout_us = ~0,
    .write_timeout_us = ~0
};


static const busart_cfg_t busart_cfg =
{
    .channel = 0,               /* USART0 */
    .baud_rate = 115200,
    .tx_size = 64,
    .rx_size = 64,
    /* Non-blocking I/O  */    
    .read_timeout_us = 0,
    .write_timeout_us = 0
};


static const cookie_io_functions_t tty_io_functions =
{
    .read = (void *)tty_read,
    .write = (void *)tty_write,
};


static const tty_cfg_t busart_tty_cfg =
{
    .read = (void *)busart_read,
    .write = (void *)busart_write,
    .linebuffer_size = 80,
};


static const tty_cfg_t usb_cdc_tty_cfg =
{
    .read = (void *)usb_cdc_read,
    .write = (void *)usb_cdc_write,
    .linebuffer_size = 80,
};


static tty_t *busart_tty;
static tty_t *usb_cdc_tty;


void
io_init (void)
{
    busart_t busart;
    usb_cdc_t usb_cdc;

    pio_config_set (LED1_PIO, PIO_OUTPUT_LOW);
    pio_config_set (LED2_PIO, PIO_OUTPUT_LOW);

    /* Initialise BUSART driver (this is a buffered USART driver that
       uses interrupts and ring buffers for efficiency).  Then create
       a TTY driver that read/writes to the BUSART driver.  The TTY
       driver echoes received characters and stores them in a
       linebuffer.  It also converts carriage return characters (sent
       by the enter key) into newlines.  Conversely, it outputs a
       carriage return character before each newline that is output.
       Characters can only be read from the linebuffer up to and
       including the next newline.  */

    busart = busart_init (&busart_cfg);
    busart_tty = tty_init (&busart_tty_cfg, busart);

    /* Initialise USB CDC driver.  Then create a TTY driver that
       read/writes to the USB CDC driver.  The TTY driver echoes
       received characters and stores them in a linebuffer.  It also
       converts carriage return characters (sent by the enter key)
       into newlines.  Conversely, it outputs a carriage return
       character before each newline that is output.  Characters can
       only be read from the linebuffer up to and including the next
       newline.  */
    usb_cdc = usb_cdc_init (&usb_cdc_cfg);
    usb_cdc_tty = tty_init (&usb_cdc_tty_cfg, usb_cdc);
}


int main (void)
{
    FILE *usart_stream;
    FILE *usb_cdc_stream;

    /* Normally, an operating system would do all this sort of stuff 
       when a computer boots.  With a bare-metal system we need to do it
       ourself.  */
    io_init ();

    /* fopencookie is non-standard GNU extension to POSIX.  */
    usb_cdc_stream = fopencookie (usb_cdc_tty, "r+", tty_io_functions);
    usart_stream = fopencookie (busart_tty, "r+", tty_io_functions);

    while (1)
    {
        int ch;

        /* These streams are non-blocking and so will normally return EOF.
           Since the TTY driver is line-buffered, characters are only
           returned if there is a newline in the line-buffer.  */
        ch = fgetc (usb_cdc_stream);
        if (ch != EOF)
        {
            /* Forward character.  */
            fputc (ch, usart_stream);
            pio_output_toggle (LED1_PIO);
        }

        ch = fgetc (usart_stream);
        if (ch != EOF)
        {
            /* Forward character.  */
            fputc (ch, usb_cdc_stream);
            pio_output_toggle (LED2_PIO);
        }
    }
}
