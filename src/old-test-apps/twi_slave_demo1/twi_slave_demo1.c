/* File:   twi_slave_demo1.c
   Author: M. P. Hayes, UCECE
   Date:   25 April 2015
   Descr:  Receive TWI packets in slave mode.
*/
#include <string.h>
#include <twi.h>
#include "target.h"


#define SLAVE_ADDR 0x42

#define MESSAGE_SIZE 16


static const twi_cfg_t twi_cfg =
{
    .slave_addr = SLAVE_ADDR
};


static void
twi_poll (twi_t twi)
{
    static char message1[MESSAGE_SIZE] = "hello";
    static char message2[MESSAGE_SIZE] = "world";
    static uint8_t addr;
    twi_ret_t ret;
    char packet[MESSAGE_SIZE + 1];

    ret = twi_slave_poll (twi);
    switch (ret)
    {
    case TWI_IDLE:
        break;
        
    case TWI_WRITE:
        ret = twi_slave_read (twi, &packet, sizeof(packet));
        if (ret < 0)
        {
            /* Error, perhaps the link was broken?  */
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
        }
        else
        {
            switch (addr)
            {
            case 1:
                memcpy (message1, packet + 1, sizeof (message1));
                break;

            case 2:
                memcpy (message2, packet + 1, sizeof (message2));
                break;

            default:
                /* Ignore packet.  */
                break;
            }
        }
        break;
        
    case TWI_READ:
        if (addr == 1)
            twi_slave_write (twi, message1, sizeof(message1));
        else if (addr == 2)
            twi_slave_write (twi, message2, sizeof(message2));
        else
        {
            /* Have a bogus address so send any old thing.  */
            twi_slave_write (twi, message1, sizeof(message1));
        }
        break;
        
    default:
        break;
    }
}


int
main (void)
{
    twi_t twi;

    twi = twi_init(&twi_cfg);

    while (1)
    {
        twi_poll (twi);
    }
}
