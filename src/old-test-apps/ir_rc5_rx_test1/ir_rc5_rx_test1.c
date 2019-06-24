/* File:   ir_rc5_rx_test1.c
   Author: M. P. Hayes, UCECE
   Date:   21 May 2013
   Descr: 
*/
#include "led.h"
#include "pio.h"
#include "ir_rc5_rx.h"


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

    /* Initialise IR driver.  */
    ir_rc5_rx_init ();

    /* Initialise LEDs.  */
    for (i = 0; i < LEDS_NUM; i++)
	leds[i] = led_init (&leds_cfg[i]);

    led_set (leds[0], 1);
    led_set (leds[1], 0);

    while (1)
    {
        int16_t data;
        
        /* Poll the IR driver.  */
        data = ir_rc5_rx_read ();
        if (data > 0)
	    led_set (leds[1], 1);
    }
}

