#ifndef PRINT_HPP
#define PRINT_HPP

namespace Wacky
{

    class Print
    {
    public:
        virtual int write (const char *buffer, unsigned int len);    


        // Print a string and append a newline.
        bool println (const char *string="");

        // Convert integer to string using desired radix and append a newline.
        bool println (int value, int radix=10);

        // Print a string.
        bool print (const char *string="");

        // Convert integer to string using desired radix.        
        bool print (int value, int radix=10);        
    };
}

#endif
