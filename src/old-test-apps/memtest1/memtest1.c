/* File:   memtest1.c
   Author: M. P. Hayes, UCECE
   Date:   15 May 2007
   Descr:  Flash an LED
*/
#include <pio.h>
#include "target.h"

#define BUFFER_SIZE (127 * 1024)


static bool
memtest (uint8_t *buffer, int size, uint8_t val)
{
    int i;

    for (i = 0; i < size; i++)
        buffer[i] = val;

    for (i = 0; i < size; i++)
        if (buffer[i] != val)
            return 0;

    return 1;
}


int
main (void)
{
    uint8_t flash_ticks;
    uint8_t buffer[BUFFER_SIZE];
    int i;
    uint8_t val = 0;

    /* Configure LED PIO as output.  */
    pio_config_set (LED1_PIO, PIO_OUTPUT_LOW);
    pio_config_set (LED2_PIO, PIO_OUTPUT_HIGH);
    
    flash_ticks = 0;

    while (1)
    {
        if (!memtest (buffer, BUFFER_SIZE, val))
            pio_output_low (LED2_PIO);
            
        val++;

        pio_output_toggle (LED1_PIO);
    }
}
