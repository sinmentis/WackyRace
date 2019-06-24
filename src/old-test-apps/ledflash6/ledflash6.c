/* File:   ledflash6.c
   Author: M. P. Hayes, UCECE
   Date:   15 May 2007
   Descr: 
*/
#include "led.h"
#include "pacer.h"

/* Define how fast ticks occur.  This must be faster than
   TICK_RATE_MIN.  */
enum {LOOP_POLL_RATE = 200};

/* Define LED flash rate in Hz.  */
enum {LED_FLASH_RATE = 2};


/* Define LEDs configuration.  */
static const led_cfg_t leds_cfg[] =
{
    {
        .pio = LED1_PIO,
        .active = 1
    },
    {
        .pio = LED2_PIO,
        .active = 1
    }
};


enum {LEDS_NUM = ARRAY_SIZE (leds_cfg)};


int
main (void)
{
    led_t leds[LEDS_NUM];
    uint8_t i;
    uint8_t flash_ticks;

    /* Initialise LEDs.  */
    for (i = 0; i < LEDS_NUM; i++)
	leds[i] = led_init (&leds_cfg[i]);

    /* Start LEDs in opposite states so they flash in anti-phase.  */
    led_set (leds[0], 0);
    led_set (leds[1], 1);

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

	    /* Toggle states of the LEDs.  */
	    led_toggle (leds[0]);
	    led_toggle (leds[1]);
	}
    }
}

