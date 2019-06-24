#ifndef USBSERIAL_HPP
#define USBSERIAL_HPP

#include "tty.h"
#include "Print.hpp"

namespace Wacky
{

    // This implements a stream connection using the USB CDC protocol.
    // By default characters are not echoed as they are read.  On
    // reading, carriage return characters are converted to newlines.
    // On writing newlines, a carriage return character is prepended.
    class UsbSerial : public Print
    {
    public:
        
        bool begin ();

        // Enable/disable echoing of characters as they arrive.
        void setEcho (bool state=true);

        // Read into buffer until first newline character or until
        // size - 1 bytes have been read.  A null terminator is added.
        // If a line is not available, false is returned.
        virtual bool readln (char *buffer, unsigned int size);

        // Write size bytes from buffer.  Returns number of bytes transferred.
        virtual int write (const char *buffer, unsigned int size);

    private:    
        tty_t *_tty;
    };
}

#endif
