/* File:   tc_output_compare_test1.c
   Author: M. P. Hayes, UCECE
   Date:   1 May 2015
   Descr:  
*/
#include "tc.h"
#include "pio.h"

#define CLOCK_FREQUENCY 10e3
#define CLOCK_PRESCALE 2

static const tc_cfg_t tc_cfg =
{
    .pio = TIOA0_PIO,
    .mode = TC_MODE_CLOCK,
    .frequency = CLOCK_FREQUENCY,
    .prescale = CLOCK_PRESCALE
};


int
main (void)
{
    tc_t tc;

    tc = tc_init (&tc_cfg);
    if (tc)
    {
        tc_start (tc);
    }    

    /* Could go to sleep here....  */
    while (1)
        continue;
}
