/* File:   extint_test1.c
   Author: M. P. Hayes, UCECE
   Date:   16 April 2013
   Descr:  This only works for the SAM7S.
*/
#include "led.h"
#include "pio.h"
#include "pacer.h"
#include "extint.h"


#define EXTINT1_PIO PA30_PIO


/* Define LED configuration.  */
static const led_cfg_t led1_cfg =
{
    .pio = LED1_PIO,
    .active = 1
};


static led_t led1;


static void handler (void)
{
    led_toggle (led1);
}

static const extint_cfg_t extint1_cfg =
{
    .pio = EXTINT1_PIO,
    .handler = handler
};


int
main (void)
{
    extint_t extint1;

    /* Initialise LED.  */
    led1 = led_init (&led1_cfg);

    /* Turn on LED.  */
    led_set (led1, 1);


    /* Note, only two PIOs can be used on the SAM7 for external interrupts.  */
    extint1 = extint_init (&extint1_cfg);

    extint_enable (extint1);

    while (1)
        continue;

    return 0;
}

