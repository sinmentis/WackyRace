#include "Adc.hpp"

namespace Wacky
{

    // The ADC peripheral is clocked at F_CPU / 4.  With a 96 MHz CPU
    // clock this is 24 MHz.  With 24 clocks per conversion, the ADC
    // can sample at 1 MHz.  The selected channels are automatically
    // multiplexed in turn (in numerical order).
    
#define IADC_CLOCK_FREQ (F_CPU / 4)
#define IADC_SAMPLE_CLOCKS 24
#define IADC_SAMPLE_FREQ (IADC_CLOCK_FREQ / IADC_SAMPLE_CLOCKS)


    // This triggers a conversion whenever adc_read is called.
    bool Adc::begin(int channels)
    {
        adc_cfg_t adc_cfg =
            {
                .channel = (adc_channel_t) 0,
                .channels = (adc_channels_t) channels,
                .bits = 12,
                .trigger = ADC_TRIGGER_SW,
                .clock_speed_kHz = (adc_clock_speed_t) IADC_CLOCK_FREQ / 1000
            };

        _adc = adc_init (&adc_cfg);
        if (! _adc)
            return 0;
        return 1;
    }
    
    unsigned Adc::read (void *buffer, size_t size)
    {
        return adc_read (_adc, buffer, size);
    }
    
}
