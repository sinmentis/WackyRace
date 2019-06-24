#include <stdio.h>
#include "usb_cdc.h"
#include "pio.h"
#include "sys.h"
#include "pacer.h"
#include "delay.h"


#define PACER_RATE 1000
#define LED_FLASH_RATE 2

#define LINEBUFFER_SIZE 80

#define GENERAL_LED_PIO  LED1_PIO
#define USB_LED_PIO  LED2_PIO
#ifdef LED3_PIO
#define BLINK_LED_PIO  LED3_PIO
#else
#define BLINK_LED_PIO  LED1_PIO
#endif


static const usb_cdc_cfg_t usb_cdc_cfg =
{
    /* Blocking I/O  */    
    .read_timeout_us = ~0,
    .write_timeout_us = ~0
};


static void
process_command (void)
{
    static int char_count = 0;
    static char linebuffer[LINEBUFFER_SIZE];
    char ch;
    

    ch = fgetc (stdin);
    fputc (ch, stderr);
    if (char_count < LINEBUFFER_SIZE - 1)
        linebuffer[char_count++] = ch;

    if (ch != '\r')
        return;
    fputc ('\n', stderr);
    linebuffer[char_count++] = 0;
    char_count = 0;

    switch (linebuffer[0])
    {
    case '0':
        pio_output_set (GENERAL_LED_PIO, 0);
        break;
        
    case '1':
        pio_output_set (GENERAL_LED_PIO, 1);
        break;

    case 'h':
        fprintf (stderr, "Hello world!\n");
        break;

    default:
       break;
    }
}


int main (void)
{
    usb_cdc_t usb_cdc;
    int flash_ticks = 0;
    bool usb_alive = 0;

    pio_config_set (GENERAL_LED_PIO, PIO_OUTPUT_LOW);                
    pio_config_set (USB_LED_PIO, PIO_OUTPUT_LOW);                
    pio_config_set (BLINK_LED_PIO, PIO_OUTPUT_LOW);                

    delay_ms (3000);
    
    usb_cdc = usb_cdc_init (&usb_cdc_cfg);

    sys_redirect_stdin ((void *)usb_cdc_read, usb_cdc);
    sys_redirect_stdout ((void *)usb_cdc_write, usb_cdc);
    sys_redirect_stderr ((void *)usb_cdc_write, usb_cdc);

    pacer_init (PACER_RATE);

    while (1)
    {
        pacer_wait ();

        if (!usb_alive && usb_cdc_update ())
        {
            usb_alive = 1;
            printf ("Hello world!\n");
            pio_output_high (USB_LED_PIO);
        }
        else if (!usb_cdc_update ())
        {
            usb_alive = 0;
            pio_output_low (USB_LED_PIO);
        }

        if (usb_cdc_read_ready_p (usb_cdc))
            process_command ();

	flash_ticks++;
	if (flash_ticks >= PACER_RATE / (LED_FLASH_RATE * 2))
	{
	    flash_ticks = 0;

	    /* Toggle LED.  */
	    pio_output_toggle (BLINK_LED_PIO);
	}

    }
}
