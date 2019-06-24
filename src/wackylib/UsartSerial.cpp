#include "busart.h"
#include "UsartSerial.hpp"

namespace Wacky
{

    bool UsartSerial::begin (channel channel, uint32_t baud_rate)
    {
        const tty_cfg_t busart_tty_cfg =
            {
            read : busart_read,
            write : busart_write
            };
        busart_cfg_t busart_cfg =
        {
            .channel = channel,
            .baud_rate = baud_rate,
            .baud_divisor = 0,
            .tx_buffer = 0,
            .rx_buffer = 0, 
            .tx_size = 64,
            .rx_size = 64,
            .read_timeout_us = 0,
            .write_timeout_us = 1000,
        };
        busart_t busart;
        
        busart = busart_init (&busart_cfg);
        
        _tty = tty_init (&busart_tty_cfg, busart);
        if (_tty == 0)
            return 0;

        // Disable echo
        setEcho (0);
        return 1;
    }

    void UsartSerial::setEcho (bool state)
    {
        tty_echo_set (_tty, state);
    }
    
    
    int UsartSerial::write (const char *buffer, unsigned int len)
    {
        return tty_write (_tty, buffer, len);
    }


    bool UsartSerial::readln (char *buffer, unsigned int size)
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
