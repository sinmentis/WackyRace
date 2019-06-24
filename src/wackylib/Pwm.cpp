#include "Pwm.hpp"

namespace Wacky
{

    bool Pwm::begin(pio_t pio, int frequency, int duty_ppt, bool invert)
    {
        pwm_cfg_t pwm_cfg =
            {
                .pio = pio,
                .period = PWM_PERIOD_DIVISOR (frequency),
                .duty = 0,
                .align = PWM_ALIGN_LEFT,
                .polarity = invert ? PWM_POLARITY_LOW : PWM_POLARITY_HIGH
            };    
        _pwm = pwm_init (&pwm_cfg);
        if (! _pwm)
            return 0;
        
        setDuty (duty_ppt);

        start ();
        
        return 1;
    }
    
    
    unsigned int Pwm::setDuty (unsigned int duty_ppt)
    {
        return pwm_duty_ppt_set (_pwm, duty_ppt);
    }
    
    
    unsigned int Pwm::setFrequency (unsigned int frequency)
    {
        return pwm_frequency_set (_pwm, frequency);
    }
    
    
    void Pwm::start ()
    {
        pwm_start (_pwm);
    }
    
    
    void Pwm::stop ()
    {
        pwm_stop (_pwm);
    }
    
}
