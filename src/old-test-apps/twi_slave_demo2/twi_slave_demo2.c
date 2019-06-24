/* File:   twi_slave_demo2.c
   Author: M. P. Hayes, UCECE
   Date:   25 April 2015
   Descr:  Receive TWI packets in slave mode.
*/
#include <string.h>
#include <twi.h>
#include "target.h"


#define SLAVE_ADDR 0x42

#define INTERNAL_ADDR 1
#define INTERNAL_ADDR_SIZE 1
#define MESSAGE_SIZE 1


static const twi_cfg_t twi_cfg =
{
    .slave_addr = SLAVE_ADDR
};


static void
twi_poll (twi_t twi)
{
    static char message[MESSAGE_SIZE];
    static uint8_t addr;
    static uint8_t value;
    twi_ret_t ret;
    char packet[MESSAGE_SIZE + INTERNAL_ADDR_SIZE];

    ret = twi_slave_poll (twi);
    switch (ret)
    {
    case TWI_IDLE:
        break;
        
    case TWI_WRITE:
        ret = twi_slave_read (twi, &packet, sizeof (packet));
        if (ret < 0)
        {
            /* Error, perhaps the link was broken?  */
            pio_output_high (LED3_PIO);
        }
        else if (ret == 1)
        {
            /* Assume one byte internal addresses.  */
            addr = packet[0];

            /* A read request will be hot on our heels.  */
            twi_poll (twi);
        }
        else if (ret != sizeof (packet))
        {
            /* Protocol error.  */
            pio_output_high (LED3_PIO);
        }
        else
        {
            value = packet[1];
            pio_output_set (LED1_PIO, value & 1);
        }
        break;
        
    case TWI_READ:
        pio_output_toggle (LED2_PIO);
        message[0] = value;
        ret = twi_slave_write (twi, message, sizeof (message));
        if (ret != sizeof (message))
            pio_output_high (LED3_PIO);
        break;
        
    default:
        break;
    }
}


int
main (void)
{
    twi_t twi;

    /* Configure LED PIOs as outputs.  */
    pio_config_set (LED1_PIO, PIO_OUTPUT_LOW);
    pio_config_set (LED2_PIO, PIO_OUTPUT_LOW);
    pio_config_set (LED3_PIO, PIO_OUTPUT_LOW);

    twi = twi_init(&twi_cfg);

    while (1)
    {
        twi_poll (twi);
    }
}
