/* File:   twi_usb_cdc_test1.c
   Author: M. P. Hayes, UCECE
   Date:   27 April 2015
   Descr:  This program requires two instances communicating via TWI
   with USB CDC operating to provide a command line interface.  By default,
   the program acts as a slave but messages can be read/written as a master
   using the r and w commands.  The s command switches back to slave mode.

   By default, the slave listens on address 66.  To send a message to
   the slave use:

   w 66 message

   To read from the slave use:

   r 66 

   This should respond with Hello world! and a number corresponding to
   the number of reads.

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "usb_cdc.h"
#include "pio.h"
#include "twi.h"
#include "sys.h"
#include "pacer.h"


#define PACER_RATE 1000

#define TIMEOUT_US 20000

#define SLAVE_ADDR 0x42

/* Choose CLOCK_SPEED depending on pullup value and bus capacitance.  */
#define CLOCK_SPEED 100e3

#define MESSAGE_SIZE 16

#define GENERAL_LED_PIO  LED1_PIO
#define USB_LED_PIO  LED2_PIO


static const twi_cfg_t twi_cfg =
{
    .period = TWI_PERIOD_DIVISOR (CLOCK_SPEED),
    .slave_addr = SLAVE_ADDR
};


static const usb_cdc_cfg_t usb_cdc_cfg =
{
    .read_timeout_us = ~0,
    .write_timeout_us = ~0
};


typedef enum {STATE_ADDR, STATE_DATA} state_t;

enum {LINEBUFFER_SIZE = 80};

static int char_count = 0;
static char linebuffer[LINEBUFFER_SIZE];
static twi_mode_t mode = TWI_MODE_SLAVE;
static twi_t twi;

static void
process_command (void)
{
    char ch;
    uint8_t addr = 0;
    char *msg;
    char message[MESSAGE_SIZE + 1];
    twi_ret_t ret;
    
    ch = fgetc (stdin);
    fputc (ch, stderr);
    if (char_count < LINEBUFFER_SIZE - 1)
        linebuffer[char_count++] = ch;

    if (ch != '\r')
        return;
    fputc ('\n', stderr);
    linebuffer[char_count++] = 0;
    char_count = 0;

    switch (linebuffer[0])
    {
    case '0':
        pio_output_set (GENERAL_LED_PIO, 0);
        break;
        
    case '1':
        pio_output_set (GENERAL_LED_PIO, 1);
        break;

    case 'h':
        fprintf (stderr, "Hello world!\n");
        break;

    case 's':
        mode = TWI_MODE_SLAVE;
        fprintf (stderr, "Slave listening on address %d\n", SLAVE_ADDR);
        break;

    case 'w':
        mode = TWI_MODE_MASTER;

        msg = linebuffer + 1;
        while (*msg == ' ')
            msg++;

        addr = strtoul (msg, &msg, 10);
        if (*msg != ' ')
        {
            fprintf (stderr, "Syntax error, w addr message\n");
            return;
        }

        while (*msg == ' ')
            msg++;

        strncpy (message, msg, MESSAGE_SIZE);
        message[MESSAGE_SIZE] = 0;
        ret = twi_master_addr_write (twi, SLAVE_ADDR, addr, 1, message,
                                     MESSAGE_SIZE);
        if (ret == MESSAGE_SIZE)
            fprintf (stderr, "Master write %d <%d>: %s\n", addr, ret, message);        
        else
            fprintf (stderr, "Master write %d: error %d\n", addr, ret);        
        break;

    case 'r':
        mode = TWI_MODE_MASTER;

        addr = strtoul (linebuffer + 1, &msg, 10);

        /* NB, this blocks while the slave gets its act together.  */
        ret = twi_master_addr_read_timeout (twi, SLAVE_ADDR, addr, 1, message,
                                            MESSAGE_SIZE, TIMEOUT_US);
        if (ret == MESSAGE_SIZE)
            fprintf (stderr, "Master read %d <%d>: %s\n", addr, ret, message);        
        else
            fprintf (stderr, "Master read %d: error %d\n", addr, ret);        
        break;
        
    case 'x':
        twi_reset (twi);
        break;

    default:
        break;
    }
}


static void
process_twi_slave (twi_t twi)
{
    static state_t state = STATE_ADDR;
    static int write_count = 0;
    static uint8_t addr = 0;
    static char message[MESSAGE_SIZE + 1];
    static char buffer[MESSAGE_SIZE + 2];
    twi_ret_t ret;

    ret = twi_slave_poll (twi);
    switch (ret)
    {
        /* Nothing doing...  */
    case TWI_IDLE:
        return;

        /* Master write.  */
    case TWI_WRITE:
        ret = twi_slave_read (twi, buffer, sizeof (buffer));
        buffer[MESSAGE_SIZE + 1] = 0;
        /* A short packet is just an address.
           A long packet is an address plus a message.  */
        addr = buffer[0];
        if (ret < 0)
        {
            fprintf (stderr, "Slave read %d: error %d\n", addr, ret);
        }
        else if (ret == 1)
        {
            fprintf (stderr, "Slave read addr %d\n", addr);

            /* Immediately check for a read message.  */
            process_twi_slave (twi);
            return;
        }
        else
        {
            fprintf (stderr, "Slave read %d <%d>: %s\n", addr, ret, buffer + 1);
        }
        break;

        /* Master read.  */
    case TWI_READ:
        snprintf (message, MESSAGE_SIZE, "Hello world! %d", write_count++);
        message[MESSAGE_SIZE] = 0;
        ret = twi_slave_write (twi, message, MESSAGE_SIZE);
        if (ret == MESSAGE_SIZE)
            fprintf (stderr, "Slave write %d <%d>: %s\n", addr, ret, message);
        else
            fprintf (stderr, "Slave write %d: error %d\n", addr, ret);
        break;
    
    default:
        fprintf (stderr, "Slave poll error %d\n", ret);
        break;
    }
}


int main (void)
{
    usb_cdc_t usb_cdc;

    pio_config_set (USB_LED_PIO, PIO_OUTPUT_LOW);                
    pio_config_set (GENERAL_LED_PIO, PIO_OUTPUT_HIGH);                

    twi = twi_init(&twi_cfg);

    usb_cdc = usb_cdc_init (&usb_cdc_cfg);
    
    sys_redirect_stdin ((void *)usb_cdc_read, usb_cdc);
    sys_redirect_stdout ((void *)usb_cdc_write, usb_cdc);
    sys_redirect_stderr ((void *)usb_cdc_write, usb_cdc);

    /* Wait until USB configured.  */
    while (! usb_cdc_update ())
        continue;
    pio_config_set (USB_LED_PIO, PIO_OUTPUT_HIGH);                

#ifdef TWI_PULLUP_PIO
    /* Actively enabled the pullups.  */
    pio_config_set (TWI_PULLUP_PIO, PIO_OUTPUT_HIGH);                
#endif

    fprintf (stderr, "Slave listening on address %d\n", SLAVE_ADDR);

    pacer_init (PACER_RATE);
    while (1)
    {
        /*  The slave will miss some reads and get screwed up if wait too long
            between reading address and sending message.  */
        // pacer_wait ();

        if (usb_cdc_read_ready_p (usb_cdc))
            process_command ();

        /* Check if USB disconnected.  */
        pio_output_set (USB_LED_PIO, usb_cdc_update ());

        if (mode == TWI_MODE_SLAVE)
            process_twi_slave (twi);
    }
}
