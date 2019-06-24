#include <stdio.h>
#include "usb_cdc.h"
#include "pio.h"
#include "sys.h"
#include "pacer.h"


#define PACER_RATE 1000


static const usb_cdc_cfg_t usb_cdc_cfg =
{
    /* Blocking I/O  */
    .read_timeout_us = ~0,
    .write_timeout_us = ~0
};


int main (void)
{
    usb_cdc_t usb_cdc;

    pio_config_set (LED1_PIO, PIO_OUTPUT_LOW);                
    pio_config_set (LED2_PIO, PIO_OUTPUT_LOW);                

    usb_cdc = usb_cdc_init (&usb_cdc_cfg);
    
    sys_redirect_stdin ((void *)usb_cdc_read, usb_cdc);
    sys_redirect_stdout ((void *)usb_cdc_write, usb_cdc);
    sys_redirect_stderr ((void *)usb_cdc_write, usb_cdc);


    /* Wait until USB configured.  */
    while (! usb_cdc_update ())
        continue;

    pacer_init (PACER_RATE);

    printf ("Hello world!\n");

    while (1)
    {
        pacer_wait ();

        if (usb_cdc_read_ready_p (usb_cdc))
        {
            char ch;

            ch = fgetc (stdin);
            fputc (ch, stderr);
            if (ch == '\r')
                fputc ('\n', stderr);

            switch (ch)
            {
            case '0':
                pio_output_set (LED2_PIO, 0);
                break;

            case '1':
                pio_output_set (LED2_PIO, 1);
                break;

            default:
                break;
            }
        }

        /* Check if USB disconnected.  */
        pio_output_set (LED1_PIO, usb_cdc_update ());
    }
}
