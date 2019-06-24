/* File:   pwm_test1.c
   Author: M. P. Hayes, UCECE
   Date:   23 Feb 2018
   Descr:  This demonstrates how to drive an A5995 dual H-bridge with PWM.
           This device requires a PWM signal for each H-bridge and a
           direction signal.
   Notes:  If a PWM signal from PB4 or PB5 is required then it necessary
           to disable JTAG functionality for these pins.

           The can be done with:

           #include "mcu.h"

           ...

           mcu_jtag_disable ();
*/
#include "UsbSerial.hpp"
#include "Pwm.hpp"
#include "stdio.h"
#include "Pio.hpp"

int main (void)
{
    Wacky::UsbSerial usbserial;
    Wacky::Pwm pwm;
    Wacky::Pio motor_sleep_pio (MOTOR_SLEEP_PIO);
    Wacky::Pio motor_left_mode_pio (MOTOR_LEFT_MODE_PIO);
    Wacky::Pio motor_right_mode_pio (MOTOR_RIGHT_MODE_PIO);
    Wacky::Pio motor_left_phase_pio (MOTOR_LEFT_PHASE_PIO);
    Wacky::Pio motor_right_phase_pio (MOTOR_RIGHT_PHASE_PIO);        
    
    /* Low to sleep.  */
    motor_sleep_pio.setMode (PIO_OUTPUT_HIGH);

    /* Set fast decay; use slow decay for braking.  */
    motor_left_mode_pio.setMode (PIO_OUTPUT_LOW);
    motor_right_mode_pio.setMode (PIO_OUTPUT_LOW);        

    /* High for forward; low for reverse.  */
    motor_left_phase_pio.setMode (PIO_OUTPUT_HIGH);
    motor_right_phase_pio.setMode (PIO_OUTPUT_HIGH);            

    pwm.begin (MOTOR_LEFT_PWM_PIO, 1000, 800);

    usbserial.begin ();
    usbserial.print ("> ");
        
    while (1)
    {
        char buffer[80];
        int speed;

        if (!usbserial.readln (buffer, sizeof (buffer)))
            continue;
            
        if (sscanf (buffer, "%d", &speed) != 1)
            continue;

        pwm.setDuty (speed * 10);
        usbserial.print ("> ");        
    }
}
