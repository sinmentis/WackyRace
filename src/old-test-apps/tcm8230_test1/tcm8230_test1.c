/* File:   tcm8230_test1.c
   Author: M. P. Hayes, UCECE
   Date:   21 Apr 2013
   Descr: 
*/
#include "tcm8230.h"


static const tcm8230_cfg_t cfg =
{
    .picsize = TCM8230_PICSIZE_SQCIF
};


static uint8_t image[SQCIF_WIDTH * SQCIF_HEIGHT * 2];

int
main (void)
{
    tcm8230_init (&cfg);

    while (1)
    {
        tcm8230_capture (image, sizeof(image), ~0);
    }
}
