/* File:   sleep_test1.c
   Author: M. P. Hayes, UCECE
   Date:   15 April 2013
   Descr:  This only works for the SAM7 and needs tweaking for the SAM4S.
*/
#include "mcu.h"
#include "mcu_sleep.h"
#include "led.h"
#include "pio.h"
#include "pacer.h"
#include "button.h"


#define BUTTON_POLL_RATE 100

/* Define LED configuration.  */
static const led_cfg_t led1_cfg =
{
    .pio = LED1_PIO,
    .active = 1
};


static const led_cfg_t led2_cfg =
{
    .pio = LED2_PIO,
    .active = 1
};


static const mcu_sleep_cfg_t sleep_cfg =
{
    .mode = MCU_SLEEP_MODE_SLEEP,
    .pio = BUTTON1_PIO,
    .active = 1
};


static const button_cfg_t button1_cfg =
{
    .pio = BUTTON1_PIO
};


int
main (void)
{
    led_t leds[2];
    button_t button1;
    bool armed = 0;

    /* Initialise LEDs.  */
    leds[0] = led_init (&led1_cfg);
    leds[1] = led_init (&led2_cfg);

    /* Turn on LEDs.  */
    led_set (leds[0], 1);
    led_set (leds[1], 1);

    /* Initialise button.  */
    button1 = button_init (&button1_cfg);

    button_poll_count_set (BUTTON_POLL_COUNT (BUTTON_POLL_RATE));

    pacer_init (BUTTON_POLL_RATE);

    while (1)
    {
        pacer_wait ();

        button_poll (button1);

        if (button_pushed_p (button1))
        {
            if (armed)
            {
                /* Turn off LED1.  */
                led_set (leds[1], 0);        

                mcu_sleep (&sleep_cfg);
            }
            armed = 1;

            /* Turn off LED1.  */
            led_set (leds[0], 0);        

            
        }
    }
    return 0;
}

