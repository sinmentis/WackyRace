/* File:   rn41_test1.c
   Author: M. P. Hayes, UCECE
   Date:   11 May 2017
   Descr:  This loops a USART to USB CDC using the buffered USART (busart) driver.
           It is useful for communicating with an RN41 bluetooth module.
*/
#include <stdio.h>
#include "busart.h"
#include "usb_cdc.h"
#include "sys.h"
#include "pio.h"
#include "delay.h"

/* TODO: These should be defined in target.h  */
#define BLUETOOTH_RN41_RESET_PIO PA11_PIO
#define BLUETOOTH_RN41_POWER_PIO PA12_PIO
#define BLUETOOTH_RN41_MASTER_PIO PA13_PIO
#define BLUETOOTH_RN41_BAUD_PIO PA14_PIO
#define BLUETOOTH_RN41_AUTO_DISCOVER_PIO PA15_PIO
#define BLUETOOTH_RN41_FACTORY_RESET_PIO PA16_PIO
#define BLUETOOTH_RN41_CTS_PIO PA24_PIO
#ifndef BLUETOOTH_RN41_USART
#define BLUETOOTH_RN41_USART 1
#endif


/* If send command $$$ then the RN41 goes into command mode.
   To exit command mode, send ---<CR>
   Useful commands: d, e, help
   In command mode, the LED blinks at 10 Hz rather than 1 Hz.
*/


static void bluetooth_factory_reset (void)
{
    pio_output_low (BLUETOOTH_RN41_FACTORY_RESET_PIO);
    delay_ms (1000);    
    pio_output_high (BLUETOOTH_RN41_FACTORY_RESET_PIO);
    delay_ms (1);

    pio_output_low (BLUETOOTH_RN41_FACTORY_RESET_PIO);
    delay_ms (1000);    
    pio_output_high (BLUETOOTH_RN41_FACTORY_RESET_PIO);
}


void rn41_init (void)
{
    #ifdef BLUETOOTH_RN41_POWER_PIO
    pio_config_set (BLUETOOTH_RN41_POWER_PIO, PIO_OUTPUT_LOW);
    
    delay_ms (10);
    
    /* Enable regulator.  */
    pio_config_set (BLUETOOTH_RN41_POWER_PIO, PIO_OUTPUT_HIGH);
    #endif

    #ifdef BLUETOOTH_RN41_RESET_PIO
    pio_config_set (BLUETOOTH_RN41_RESET_PIO, PIO_OUTPUT_HIGH);
    #endif
    
    pio_config_set (BLUETOOTH_RN41_FACTORY_RESET_PIO, PIO_OUTPUT_HIGH);
    /* Set 115200 baud rate.  */
    pio_config_set (BLUETOOTH_RN41_BAUD_PIO, PIO_OUTPUT_LOW);
    /* This must be low if you want to pair with the RN41.  */
    pio_config_set (BLUETOOTH_RN41_MASTER_PIO, PIO_OUTPUT_LOW);
    pio_config_set (BLUETOOTH_RN41_AUTO_DISCOVER_PIO, PIO_OUTPUT_HIGH);

    /* Wait for power to come up...  */
    delay_ms (10);    
    
    #ifdef BLUETOOTH_RN41_RESET_PIO
    pio_config_set (BLUETOOTH_RN41_RESET_PIO, PIO_OUTPUT_LOW);
    delay_ms (1);
    pio_config_set (BLUETOOTH_RN41_RESET_PIO, PIO_OUTPUT_HIGH);    
    #endif

    /* Hack: Force CTS for USART1 low.  The RN41 will not send
       anything over the USART while this is high.  */
    pio_config_set (BLUETOOTH_RN41_CTS_PIO, PIO_OUTPUT_LOW);
}


static const usb_cdc_cfg_t usb_cdc_cfg =
{
    /* Non-blocking I/O  */    
    .read_timeout_us = 0,
    .write_timeout_us = 0
};


static const busart_cfg_t busart_cfg =
{
    .channel = BLUETOOTH_RN41_USART,
    .baud_rate = 115200,
    .tx_size = 64,
    .rx_size = 64
};


int main (void)
{
    char txbuffer[64];
    char rxbuffer[64];
    busart_t busart;
    usb_cdc_t usb_cdc;

    pio_config_set (LED1_PIO, PIO_OUTPUT_LOW);
    pio_config_set (LED2_PIO, PIO_OUTPUT_LOW);

    rn41_init ();

    busart = busart_init (&busart_cfg);
    usb_cdc = usb_cdc_init (&usb_cdc_cfg);
    
    sys_redirect_stdin ((void *)usb_cdc_read, usb_cdc);
    sys_redirect_stdout ((void *)usb_cdc_write, usb_cdc);
    sys_redirect_stderr ((void *)usb_cdc_write, usb_cdc);

    /* Wait until USB configured.  */
    while (! usb_cdc_update ())
        continue;

    pio_config_set (LED1_PIO, PIO_OUTPUT_HIGH);

    delay_ms (5000);

    pio_config_set (LED2_PIO, PIO_OUTPUT_HIGH);

    
    while (1)
    {
        if (usb_cdc_read_ready_p (usb_cdc))
        {
            char ch;

            ch = fgetc (stdin);
            busart_putc (busart, ch);

            /* Echo character.  */
            fputc (ch, stderr);
        }

        if (busart_read_ready_p (busart))
        {
            char ch;

            ch = busart_getc (busart);

            fputc (ch, stderr);
        }
    }
}
