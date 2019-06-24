/* File:   ledflash5.c
   Author: M. P. Hayes, UCECE
   Date:   15 May 2007
   Descr: 
*/
#include <pio.h>
#include "target.h"
#include "pacer.h"

/* Define how fast ticks occur.  This must be faster than
   TICK_RATE_MIN.  */
enum {LOOP_POLL_RATE = 200};

/* Define LED flash rate in Hz.  */
enum {LED_FLASH_RATE = 2};


int
main (void)
{
    uint8_t flash_ticks;

    /* Configure PIOs as outputs for LED1 and LED2.  */
    pio_config_set (LED1_PIO, PIO_OUTPUT_LOW);
    pio_config_set (LED2_PIO, PIO_OUTPUT_LOW);
    
    /* Turn on LED1 and turn off LED2.  Note LEDs connected between pio
       and Vcc so a logic low output turns the LEDs on.  */
    pio_output_low (LED1_PIO);
    pio_output_high (LED2_PIO);

    pacer_init (LOOP_POLL_RATE);
    flash_ticks = 0;

    while (1)
    {
	/* Wait until next clock tick.  */
	pacer_wait ();

	flash_ticks++;
	if (flash_ticks >= LOOP_POLL_RATE / (LED_FLASH_RATE * 2))
	{
	    flash_ticks = 0;

	    /* Toggle LEDs.  */
	    pio_output_toggle (LED1_PIO);
	    pio_output_toggle (LED2_PIO);
	}
    }
}
