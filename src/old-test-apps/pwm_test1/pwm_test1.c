/* File:   pwm_test1.c
   Author: M. P. Hayes, UCECE
   Date:   15 April 2013
   Descr: 
*/
#include "pwm.h"
#include "pio.h"

#define PWM1_PIO PA1_PIO

#define PWM1_FREQ_HZ 100e3


static const pwm_cfg_t pwm1_cfg =
{
    .pio = PWM1_PIO,
    .frequency = PWM1_FREQ_HZ,
    /* Duty in parts per thousand.  */
    .duty_ppt = 500,
    .align = PWM_ALIGN_LEFT,
    .polarity = PWM_POLARITY_LOW
};


int
main (void)
{
    pwm_t pwm1;

    pwm1 = pwm_init (&pwm1_cfg);

    pwm_start (pwm1);

    while (1)
        continue;
    
    return 0;
}
