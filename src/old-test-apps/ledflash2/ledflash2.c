/* File:   ledflash2.c
   Author: M. P. Hayes, UCECE
   Date:   23 Feb 2015
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
    },
    {
        .pio = LED3_PIO,
        .active = 1
    }
};


enum {LEDS_NUM = ARRAY_SIZE (leds_cfg)};


int
main (void)
{
    led_t leds[LEDS_NUM];
    uint8_t i;
    int flash_ticks;
    int seq = 0;

    /* Initialise LEDs.  */
    for (i = 0; i < LEDS_NUM; i++)
	leds[i] = led_init (&leds_cfg[i]);

    led_set (leds[seq], 1);

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

	    led_set (leds[seq], 0);
            
            seq++;
            if (seq >= LEDS_NUM)
                seq = 0;

	    led_set (leds[seq], 1);
	}
    }
}

