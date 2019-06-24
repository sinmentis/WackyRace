/* File:   usb_tty.c
   Author: M. P. Hayes, UCECE
   Date:   12 May 2016
*/

#include "usb_cdc.h"
#include "usb_tty.h"

static const usb_cdc_cfg_t usb_cdc_cfg =
{
    /* Non-blocking I/O  */
    .read_timeout_us = 0,
    .write_timeout_us = 0    
};


static const tty_cfg_t usb_cdc_tty_cfg =
{
    .read = (void *)usb_cdc_read,
    .write = (void *)usb_cdc_write
};


tty_t *
usb_tty_init (void)
{
    usb_cdc_t usb_cdc;

    usb_cdc = usb_cdc_init (&usb_cdc_cfg);

    return tty_init (&usb_cdc_tty_cfg, usb_cdc);
}


