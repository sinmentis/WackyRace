#ifndef PACER_HPP
#define PACER_HPP

#include "pacer.h"

namespace Wacky
{

    class Pacer
    {
    public:
        Pacer(unsigned int frequency)
            {
                pacer_init (frequency);
            }
        
        void wait ()
            {
                pacer_wait ();
            }
    };
}


#endif
