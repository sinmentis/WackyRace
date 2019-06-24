/* File:   pio_interrupt_demo1.c
   Author: M. P. Hayes, UCECE
   Date:   1 May 2015
   Descr:  
*/
#include "pio.h"
#include "irq.h"
#include "target.h"
#include "pacer.h"

/* Define how fast ticks occur.  This must be faster than
   TICK_RATE_MIN.  */
enum {LOOP_POLL_RATE = 200};

/* Define LED flash rate in Hz.  */
enum {LED_FLASH_RATE = 2};


#ifndef WAKEUP_PIO
#define WAKEUP_PIO PA5_PIO
#endif


static void
pio_handler (void)
{
   /* Read ISR for the PIO to clear the interrupt.  NB, this clears
      all the interrupts on the same PIO port.  */

    pio_irq_clear (WAKEUP_PIO);
    irq_disable (PIO_ID (WAKEUP_PIO));

    pio_output_high (LED2_PIO);
}


int
main (void)
{
    uint8_t flash_ticks;

    /* Configure LED PIO as output.  */
    pio_config_set (LED1_PIO, PIO_OUTPUT_LOW);
    pio_config_set (LED2_PIO, PIO_OUTPUT_LOW);

    /* This ensures the clock is running for the PIO controller;
       needed to read an input pin.  */
    pio_init (WAKEUP_PIO);
    pio_config_set (WAKEUP_PIO, PIO_INPUT);

    pio_irq_config_set (WAKEUP_PIO, PIO_IRQ_FALLING_EDGE);

    irq_config (PIO_ID (WAKEUP_PIO), 1, pio_handler);
    
    irq_enable (PIO_ID (WAKEUP_PIO));

    pio_irq_enable (WAKEUP_PIO);

    
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

	    /* Toggle LED.  */
	    pio_output_toggle (LED1_PIO);
	}
    }
}
