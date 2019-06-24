/* File:   adc_test1.cpp
   Author: M. P. Hayes, UCECE
   Date:   25 Feb 2018
   Descr:  This triggers ADC conversions on two channels in software
            as each sample is read.
*/

#include "Led.hpp"
#include "Adc.hpp"
#include "Pacer.hpp"
#include "UsbSerial.hpp"

/* Number of samples to read each time.  
   The first sample is from AD5 and the second is from AD6.  */
#define ADC_SAMPLES 3

int main (void)
{
    Wacky::Led led1 (LED1_PIO, Wacky::ActiveLow);
    Wacky::Led led2 (LED2_PIO, Wacky::ActiveLow);
    Wacky::Pacer pacer(10);
    Wacky::UsbSerial usbserial;
    Wacky::Adc adc;
    unsigned int count = 0;
    
    led1 = 1;
    led2 = 0;

    usbserial.begin ();

    adc.begin (BIT (ADC_CHANNEL_1) | BIT (ADC_CHANNEL_2) | BIT (ADC_CHANNEL_3));
    
    while (1)
    {
        int16_t buffer[ADC_SAMPLES];
        int8_t voltage[1];
        int bytes;

        pacer.wait ();

        led2.toggle ();
        
        /* Read raw 12-bit unsigned data.  */
        bytes = adc.read (buffer, sizeof (buffer));
		buffer[0] = buffer[0] / 22.78 - 90; // 4100/180=22.78
		buffer[1] = buffer[1] / 22.78 - 90;
        count++;
        usbserial.print (count);
        usbserial.print (": ");
        usbserial.print (buffer[0]);
        usbserial.print (", ");        
        usbserial.println (buffer[1]);
        usbserial.print (", ");        
        voltage[0] = buffer[2];
        usbserial.println (voltage[0]);
    }
}
