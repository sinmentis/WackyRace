/* File:   usb_tty.h
   Author: M. P. Hayes, UCECE
   Date:   12 May 2016
*/

#ifndef USB_TTY
#define USB_TTY

#ifdef __cplusplus
extern "C" {
#endif
    

#include "tty.h"

tty_t *usb_tty_init (void);


#ifdef __cplusplus
}
#endif    
#endif


