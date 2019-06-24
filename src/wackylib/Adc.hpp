#ifndef ADC_HPP
#define ADC_HPP

#include "pio.h"
#include "adc.h"

namespace Wacky
{

    class Adc
    {
    public:
        bool begin(int channels);
        
        unsigned read (void *buffer, size_t size);        
        
    private:
        adc_t _adc;
    };
}

#endif
