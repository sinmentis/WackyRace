/* File:   adc_usb_cdc_test1.c
   Author: M. P. Hayes, UCECE
   Date:   14 May 2015
   Descr:  This triggers ADC conversions on two channels in software
            as each sample is read.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "usb_cdc.h"
#include "pio.h"
#include "adc.h"
#include "sys.h"
#include "pacer.h"


#define PACER_RATE 2

#define GENERAL_LED_PIO  LED1_PIO
#define USB_LED_PIO  LED2_PIO

/* Number of samples to read each time.  
   The first sample is from AD5 and the second is from AD6.  */
#define ADC_SAMPLES 2

#define IADC_CLOCK_FREQ (F_CPU / 4)
#define IADC_SAMPLE_CLOCKS 24
#define IADC_SAMPLE_FREQ (IADC_CLOCK_FREQ / IADC_SAMPLE_CLOCKS)


static const usb_cdc_cfg_t usb_cdc_cfg =
{
    /* Blocking I/O  */    
    .read_timeout_us = ~0,
    .write_timeout_us = ~0    
};


static const adc_cfg_t adc_cfg =
{
    .bits = 12,
    .channels = BIT (ADC_CHANNEL_5) | BIT (ADC_CHANNEL_6),
    .trigger = ADC_TRIGGER_SW,
    .clock_speed_kHz = IADC_CLOCK_FREQ / 1000
};


int main (void)
{
    usb_cdc_t usb_cdc;
    int count = 0;
    adc_t adc;    

    pio_config_set (USB_LED_PIO, PIO_OUTPUT_LOW);                
    pio_config_set (GENERAL_LED_PIO, PIO_OUTPUT_HIGH);                

    adc = adc_init (&adc_cfg);    

    usb_cdc = usb_cdc_init (&usb_cdc_cfg);
    
    sys_redirect_stdin ((void *)usb_cdc_read, usb_cdc);
    sys_redirect_stdout ((void *)usb_cdc_write, usb_cdc);
    sys_redirect_stderr ((void *)usb_cdc_write, usb_cdc);

    /* Wait until USB configured.  */
    while (! usb_cdc_update ())
        continue;
    pio_config_set (USB_LED_PIO, PIO_OUTPUT_HIGH);                

    pacer_init (PACER_RATE);
    while (1)
    {
        int i;
        int16_t buffer[ADC_SAMPLES];
        int bytes;

        pacer_wait ();

        /* Check if USB disconnected.  */
        pio_output_set (USB_LED_PIO, usb_cdc_update ());

        pio_output_toggle (GENERAL_LED_PIO);

        /* Read raw 12-bit unsigned data.  */
        bytes = adc_read (adc, buffer, sizeof (buffer));

        printf ("*** %4d *** \n", count);
        count++;
        for (i = 0; i < ADC_SAMPLES; i++)
            printf ("%3d: %d\n", i, buffer[i]);
    }
}
