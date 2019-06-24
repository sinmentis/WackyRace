#ifndef PWM_HPP
#define PWM_HPP

#include "pio.h"
#include "pwm.h"

namespace Wacky
{

    class Pwm
    {
    public:
        bool begin(pio_t pio, int frequency, int duty_ppt, bool invert=false);

        // Set the PWM duty cycle in parts per thousand.
        unsigned int setDuty (unsigned int duty_ppt);

        // Set the PWM frequency in Hertz.        
        unsigned int setFrequency (unsigned int frequency);

        // Start PWM.
        void start ();

        // Stop PWM.        
        void stop ();        
        
    private:
        pwm_t _pwm;
    };
}

#endif
