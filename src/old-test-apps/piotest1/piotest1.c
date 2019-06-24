/* File:   piotest1.c
   Author: M. P. Hayes, UCECE
   Date:   15 May 2007
   Descr: 
*/
#include <pio.h>
#include "target.h"


int
main (void)
{
    /* Configure PIOs as outputs for LED1 and LED2.  */
    pio_config_set (LED1_PIO, PIO_OUTPUT_LOW);
    pio_config_set (LED2_PIO, PIO_OUTPUT_HIGH);
    
    while (1)
    {
        pio_output_toggle (LED1_PIO);
        pio_output_toggle (LED2_PIO);
    }
}
