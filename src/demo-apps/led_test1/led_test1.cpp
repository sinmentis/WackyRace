/* File:   led_test1.c
   Author: M. P. Hayes, UCECE
   Date:   25 Feb 2018
   Descr: 
*/
#include "Led.hpp"
#include "Pacer.hpp"

#define PACER_RATE 10

int main (void)
{
    Wacky::Led led1 (LED1_PIO, Wacky::ActiveLow);
    Wacky::Led led2 (LED2_PIO, Wacky::ActiveLow);
    Wacky::Pacer pacer (PACER_RATE);    

    led1 = 1;
    
    while (1)
    {
        pacer.wait ();

        led2.toggle ();
    }
}
