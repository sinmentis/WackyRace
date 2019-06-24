/* File:   twi_master_demo2.c
   Author: M. P. Hayes, UCECE
   Date:   25 April 2015
   Descr:  Send TWI packet and hope slave echoes it.
*/
#include <pio.h>
#include <twi.h>
#include "target.h"
#include "pacer.h"


enum {LOOP_POLL_RATE = 2};

#define SLAVE_ADDR 0x42
#define INTERNAL_ADDR 1
#define INTERNAL_ADDR_SIZE 1
#define MESSAGE_SIZE 1

#define CLOCK_SPEED 100e3


static const twi_cfg_t twi_cfg =
{
    .period = TWI_PERIOD_DIVISOR (CLOCK_SPEED)
};


int
main (void)
{
    twi_t twi;
    twi_ret_t ret;
    uint8_t message[MESSAGE_SIZE];
    uint8_t sent_value = 0;
    uint8_t recv_value = 0;

    /* Configure LED PIOs as outputs.  */
    pio_config_set (LED1_PIO, PIO_OUTPUT_LOW);
    pio_config_set (LED2_PIO, PIO_OUTPUT_LOW);
    pio_config_set (LED3_PIO, PIO_OUTPUT_LOW);

    twi = twi_init (&twi_cfg);

    pacer_init (LOOP_POLL_RATE);

    while (1)
    {
	/* Wait until next clock tick.  */
	pacer_wait ();

        message[0] = sent_value;

        pio_output_set (LED1_PIO, sent_value & 1);

        ret = twi_master_addr_write (twi, SLAVE_ADDR, INTERNAL_ADDR, 
                                     INTERNAL_ADDR_SIZE, 
                                     message, sizeof (message));

        message[0] = 255;
        ret = twi_master_addr_read (twi, SLAVE_ADDR, INTERNAL_ADDR, 
                                    INTERNAL_ADDR_SIZE, 
                                    message, sizeof (message));
        recv_value = message[0];


        /* Turn LED on if have error.  */
        pio_output_set (LED3_PIO, (ret != 1) || (sent_value != recv_value));
            
        pio_output_set (LED2_PIO, recv_value & 1);
    }
}
