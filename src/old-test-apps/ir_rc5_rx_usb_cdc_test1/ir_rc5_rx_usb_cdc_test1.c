/* File:   ir_rc5_usb_cdc_test1.c
   Author: M. P. Hayes, UCECE
   Date:   18 May 2017
   Descr: 
*/
#include <stdio.h>
#include "usb_cdc.h"
#include "pio.h"
#include "ir_rc5_rx.h"


static const usb_cdc_cfg_t usb_cdc_cfg =
{
    /* Blocking I/O  */
    .read_timeout_us = ~0,
    .write_timeout_us = ~0
};


int
main (void)
{
    usb_cdc_t usb_cdc;

    usb_cdc = usb_cdc_init (&usb_cdc_cfg);
    
    sys_redirect_stdin ((void *)usb_cdc_read, usb_cdc);
    sys_redirect_stdout ((void *)usb_cdc_write, usb_cdc);
    sys_redirect_stderr ((void *)usb_cdc_write, usb_cdc);    

    /* Initialise IR driver.  */
    ir_rc5_rx_init ();

    /* Wait until USB configured.  */
    while (! usb_cdc_update ())
        continue;
    
    while (1)
    {
        int16_t data;
        
        /* Poll the IR driver.  */
        data = ir_rc5_rx_read ();
        if (data > 0)
            printf ("%08x\n", data);
    }
}

