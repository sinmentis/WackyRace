/* File:   usb_cdc_tty_twi1.c
   Author: M. P. Hayes, UCECE
   Date:   27 May 2017

   Descr: This loops a USART to USB CDC using the buffered USART
           (twi) driver.  It is useful for communicating with a
           serial device, such as a bluetooth module.  I/O is
           performed using the C library with fopencookie.  It uses a
           TTY driver to provide line buffering and
           carriage-return/newline handling. 
*/
#include <stdio.h>
#include "twi.h"
#include "usb_cdc.h"
#include "sys.h"
#include "pio.h"
#include "tty.h"
#include "delay.h"

#ifndef TWI_CLOCK_SPEED
#define TWI_CLOCK_SPEED 10e3
#endif


static const usb_cdc_cfg_t usb_cdc_cfg =
{
    /* Non-blocking I/O  */    
    .read_timeout_us = 0,
    .write_timeout_us = 0
};


static const twi_cfg_t twi_cfg =
{
    .period = TWI_PERIOD_DIVISOR (TWI_CLOCK_SPEED)
};


static const cookie_io_functions_t tty_io_functions =
{
    .read = (void *)tty_read,
    .write = (void *)tty_write,
};


static const tty_cfg_t usb_cdc_tty_cfg =
{
    .read = (void *)usb_cdc_read,
    .write = (void *)usb_cdc_write,
    .linebuffer_size = 80,
};


static tty_t *usb_cdc_tty;


void
io_init (void)
{
    usb_cdc_t usb_cdc;

    pio_config_set (LED1_PIO, PIO_OUTPUT_LOW);
    pio_config_set (LED2_PIO, PIO_OUTPUT_LOW);

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
    FILE *usb_cdc_stream;
    twi_t twi;
    

    /* Normally, an operating system would do all this sort of stuff 
       when a computer boots.  With a bare-metal system we need to do it
       ourself.  */
    io_init ();

    /* fopencookie is non-standard GNU extension to POSIX.  */
    usb_cdc_stream = fopencookie (usb_cdc_tty, "r+", tty_io_functions);

    twi = twi_init (&twi_cfg);
    
    while (1)
    {
        char buffer[80];
        unsigned int slave_addr;
        unsigned int addr;
        unsigned int ivalue;
        uint8_t value;
        int8_t status;

        /* These streams are non-blocking and so will normally return EOF.
           Since the TTY driver is line-buffered, characters are only
           returned if there is a newline in the line-buffer.  */
        if (!fgets (buffer, sizeof (buffer), usb_cdc_stream))
            continue;

        pio_output_toggle (LED1_PIO);        
        
        if (buffer[0] == '\n')
        {
            fputs ("W slave-addr addr value\n"
                   "R slave-addr addr\n", usb_cdc_stream);
            continue;
        }

        if (buffer[0] == 'w' || buffer[0] == 'W')
        {
            sscanf (buffer + 1, "%i %i %i", &slave_addr, &addr, &ivalue);
            value = ivalue;
            status = twi_master_addr_write (twi, slave_addr,
                                            addr, 1, &value, 1);
            fprintf (usb_cdc_stream, "status = %d\n", status);
            continue;
        }

        if (buffer[0] == 'r' || buffer[0] == 'R')
        {
            sscanf (buffer + 1, "%i %i", &slave_addr, &addr);
            value = 0;
            status = twi_master_addr_read (twi, slave_addr,
                                           addr, 1, &value, 1);
            fprintf (usb_cdc_stream, "status = %d, value = %d\n", status, value);
            continue;
        }

        fprintf (usb_cdc_stream, "Unknown command: %s\n", buffer);
    }
}
