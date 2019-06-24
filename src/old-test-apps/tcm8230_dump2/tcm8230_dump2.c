/* File:   tcm8230_dump2.c
   Author: M. P. Hayes, UCECE
   Date:   27 Apr 2013
   Descr: 
*/
#include "tcm8230.h"
#include <stdio.h>
#include "busart.h"
#include "sys.h"
#include "led.h"


static const busart_cfg_t busart_cfg =
{
    .channel = 0,               /* USART0 */
    .baud_rate = 115200,
    .tx_size = 64,
    .rx_size = 64
};


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
    .picsize = TCM8230_PICSIZE_SQCIF
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
    busart_t busart;
    int i;

    /* Initialise LEDs.  */
    for (i = 0; i < LEDS_NUM; i++)
	leds[i] = led_init (&leds_cfg[i]);

    led_set (leds[0], 0);
    led_set (leds[1], 0);
    led_set (leds[2], 0);

    busart = busart_init (&busart_cfg);

    sys_redirect_stdin ((void *)busart_read, busart);
    sys_redirect_stdout ((void *)busart_write, busart);
    sys_redirect_stderr ((void *)busart_write, busart);

    tcm8230_init (&cfg);

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

        led_set (leds[0], 1);
        led_set (leds[2], 1);

        if (ret < 0)
            fprintf (stderr, "TCM8230 error: %d\r\n", (int)ret);
        else
            image_dump(image, SQCIF_HEIGHT, SQCIF_WIDTH);
        led_set (leds[2], 0);
    }
}
