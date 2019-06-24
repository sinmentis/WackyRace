/* File:   clock_test1.c
   Author: M. P. Hayes, UCECE
   Date:   26 Apr 2018
   Descr:  This generates a 50% duty cycle 12 MHz signal to check the CPU 
           clock frequency.
*/
#include "Pwm.hpp"
#include "Led.hpp"
#include "Pacer.hpp"

#define PACER_RATE 10

int main (void)
{
    Wacky::Pwm pwm;
    Wacky::Led led2 (LED2_PIO, Wacky::ActiveLow);
    Wacky::Pacer pacer (PACER_RATE);    

    pwm.begin (MOTOR_LEFT_PWM_PIO, 12000000, 500);
    
    while (1)
    {
        pacer.wait ();

        led2.toggle ();
    }
}
