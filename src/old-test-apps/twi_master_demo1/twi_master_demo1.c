/* File:   twi_master_demo1.c
   Author: M. P. Hayes, UCECE
   Date:   25 April 2015
   Descr:  Send TWI packet
*/
#include <pio.h>
#include <twi.h>
#include "target.h"
#include "pacer.h"


enum {LOOP_POLL_RATE = 2};

#define SLAVE_ADDR 0x42

#define CLOCK_SPEED 100e3


static const twi_cfg_t twi_cfg =
{
    .period = TWI_PERIOD_DIVISOR (CLOCK_SPEED)
};


int
main (void)
{
    char message1[32] = "Hello world";
    char message2[32] = "Testing 1,2, 3...";
    twi_t twi;

    /* Configure LED PIO as output.  */
    pio_config_set (LED1_PIO, PIO_OUTPUT_LOW);

    twi = twi_init(&twi_cfg);

    pacer_init (LOOP_POLL_RATE);

    while (1)
    {
	/* Wait until next clock tick.  */
	pacer_wait ();

        twi_master_addr_write (twi, SLAVE_ADDR, 1, 1, message1, sizeof(message1));
        twi_master_addr_write (twi, SLAVE_ADDR, 2, 1, message2, sizeof(message2));
        pio_output_toggle (LED1_PIO);
    }
}
