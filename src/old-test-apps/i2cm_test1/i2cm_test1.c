/* File:   i2cm_test1.c
   Author: M. P. Hayes, UCECE
   Date:   13 April 2013
   Descr:  This tests bit-bashed I2C as a master.
*/
#include "i2c_master.h"
#include "pacer.h"

#define SDA_PIO PA3_PIO
#define SCL_PIO PA4_PIO

#define PACER_RATE 10

#define SLAVE_ADDR 0x32

static const i2c_bus_cfg_t i2c_bus_cfg =
{
    .scl = SCL_PIO,
    .sda = SDA_PIO
};


int
main (void)
{
    i2c_t i2c_slave1;

    i2c_slave1 = i2c_master_init (&i2c_bus_cfg);

    pacer_init (PACER_RATE);

    while (1)
    {
        uint8_t tx[] = {1, 2, 3, 4};
        uint8_t rx[] = {0, 0, 0, 0};
        i2c_addr_t addr = 1;

        pacer_wait ();

        i2c_master_addr_write (i2c_slave1, SLAVE_ADDR, addr, 1, tx, sizeof(tx));
    }
}

