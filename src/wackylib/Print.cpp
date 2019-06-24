#include "Print.hpp"
#include <string.h>
#include <stdlib.h>

namespace Wacky
{


    bool Print::print (const char *string)
    {
        if (this->write (string, strlen (string)) < 0)
            return 0;
        
        return 1;
    }
    

    bool Print::println (const char *string)
    {
        if (! print (string))
            return 0;
        if (! print ("\n"))
            return 0;    
        return 1;
    }
    
    
    bool Print::print (int value, int radix)
    {
        char buffer[16];
        
        itoa (value, buffer, radix);
        return print (buffer);
    }
    
    
    bool Print::println (int value, int radix)
    {
        char buffer[16];
        
        itoa (value, buffer, radix);
        return println (buffer);
    }
}

