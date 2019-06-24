/* File:   tcm8230_dump1.c
   Author: M. P. Hayes, UCECE
   Date:   21 Apr 2013
   Descr: 
*/
#include "tcm8230.h"
#include <stdio.h>
#include "usb_cdc.h"
#include "sys.h"
#include "led.h"


/* Define LEDs configuration.  */
static const led_cfg_t leds_cfg[] =
{
    {
        .pio = LED1_PIO,
        .active = 0
    },
    {
        .pio = LED2_PIO,
        .active = 0
    },
    {
        .pio = LED3_PIO,
        .active = 0
    }
};

enum {LEDS_NUM = ARRAY_SIZE (leds_cfg)};


static const tcm8230_cfg_t cfg =
{
    .picsize = TCM8230_PICSIZE_SQCIF,
    .colour = 1
};


static const usb_cdc_cfg_t usb_cdc_cfg =
{
    /* Blocking I/O  */    
    .read_timeout_us = ~0,
    .write_timeout_us = ~0
};


static uint8_t image[SQCIF_WIDTH * SQCIF_HEIGHT * 2];


static void image_dump (uint8_t *image, unsigned int rows, unsigned int cols)
{
    unsigned int row;
    unsigned int col;

    printf ("**************\r\n");
    for (row = 0; row < rows; row++)
    {
        printf ("%3d: ", row);
        for (col = 0; col < cols * 2; col++)
            printf ("%3d, ", image[row * cols * 2 + col]);
        printf ("\r\n");
    }
    printf ("\r\n");
}


int
main (void)
{
    led_t leds[LEDS_NUM];
    usb_cdc_t usb_cdc;
    int i;

    /* Initialise LEDs.  */
    for (i = 0; i < LEDS_NUM; i++)
	leds[i] = led_init (&leds_cfg[i]);

    led_set (leds[0], 0);
    led_set (leds[1], 0);
    led_set (leds[2], 0);

    usb_cdc = usb_cdc_init (&usb_cdc_cfg);
    
    sys_redirect_stdin ((void *)usb_cdc_read, usb_cdc);
    sys_redirect_stdout ((void *)usb_cdc_write, usb_cdc);
    sys_redirect_stderr ((void *)usb_cdc_write, usb_cdc);

    tcm8230_init (&cfg);

    /* Wait until USB configured.  */
    while (! usb_cdc_update ())
        continue;

    led_set (leds[0], 1);

    while (1)
    {
        int32_t ret;

        led_set (leds[1], 1);
        while ( tcm8230_frame_ready_p ())
            continue;

        led_set (leds[1], 0);
        while (! tcm8230_frame_ready_p ())
            continue;

        led_set (leds[1], 1);

        ret = tcm8230_capture (image, sizeof(image), 200);

        if (usb_cdc_update ())
        {
            led_set (leds[0], 1);
            led_set (leds[2], 1);

            if (ret < 0)
                fprintf (stderr, "TCM8230 error: %d\r\n", (int)ret);
            else
                image_dump(image, SQCIF_HEIGHT, SQCIF_WIDTH);
            led_set (leds[2], 0);
        }
        else
        {
            /* USB disconnected.  */
            led_set (leds[0], 0);
        }
    }
}
