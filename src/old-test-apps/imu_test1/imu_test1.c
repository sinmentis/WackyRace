/* File:   imu_test1.c
   Author: M. P. Hayes, UCECE
   Date:   20 Feb 2018
   Descr:  
*/
#include <pio.h>
#include "target.h"
#include "pacer.h"
#include "mpu9250.h"


enum {LOOP_POLL_RATE = 10};


int
main (void)
{
    mpu9250_t *mpu9250 = 0;
    
    /* Configure LED PIOs as outputs.  */
    pio_config_set (LED1_PIO, PIO_OUTPUT_LOW);
    pio_config_set (LED2_PIO, PIO_OUTPUT_LOW);

    do
    {
        mpu9250 = mpu9250_init ();
        pio_output_toggle (LED1_PIO);
        pio_output_toggle (LED2_PIO);
    } while (mpu9250 == 0);

    pio_output_low (LED1_PIO);
    pio_output_high (LED2_PIO);
    
    pacer_init (LOOP_POLL_RATE);

    while (1)
    {
        
	/* Wait until next clock tick.  */
	pacer_wait ();

        pio_output_toggle (LED1_PIO);
        pio_output_toggle (LED2_PIO);
    }
}
