#include "UsbSerial.hpp"
#include "usb_cdc.h"

namespace Wacky
{

    bool UsbSerial::begin ()
    {
        const usb_cdc_cfg_t usb_cdc_cfg =
            {
                /* Non-blocking I/O  */    
                .read_timeout_us = 0,
                .write_timeout_us = 0
            };
        const tty_cfg_t usb_cdc_tty_cfg =
            {
                .read = usb_cdc_read,
                .write = usb_cdc_write
            };
        usb_cdc_t usb_cdc;

        usb_cdc = usb_cdc_init (&usb_cdc_cfg);        
        
        _tty = tty_init (&usb_cdc_tty_cfg, usb_cdc);
        return _tty != 0;
    }
    
    
    void UsbSerial::setEcho (bool state)
    {
        tty_echo_set (_tty, state);
    }


    int UsbSerial::write (const char *buffer, unsigned int len)
    {
        return tty_write (_tty, buffer, len);
    }
    
    
    bool UsbSerial::readln (char *buffer, unsigned int size)
    {
        ssize_t ret;
        
        buffer[0] = 0;

        ret = tty_read (_tty, buffer, size - 1);
        if (ret < 0)
            return false;
        buffer[ret] = 0;
        return true;
    }

}
