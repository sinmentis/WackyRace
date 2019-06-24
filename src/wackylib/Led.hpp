#ifndef LED_HPP
#define LED_HPP

#include "Pio.hpp"

namespace Wacky
{
    enum polarity {ActiveLow, ActiveHigh};
    
    class Led
    {
        polarity _polarity;
        Wacky::Pio *_pio;
        
    public:
        Led (pio_t pio, polarity polarity)
            {
                _pio = new Wacky::Pio(pio);
                _polarity = polarity;
                _pio->setMode (polarity == ActiveHigh ? PIO_OUTPUT_HIGH : PIO_OUTPUT_LOW);
            }

        Led (Pio *pio, polarity polarity)
            {
                _pio = pio;
                _polarity = polarity;
                _pio->setMode (polarity == ActiveHigh ? PIO_OUTPUT_HIGH : PIO_OUTPUT_LOW);
            }

        Led (Pio pio, bool active_high=true)
            {
                _pio = new Wacky::Pio(pio);                
                _polarity = active_high ? ActiveHigh : ActiveLow;
                _pio->setMode (_polarity == ActiveHigh ? PIO_OUTPUT_HIGH : PIO_OUTPUT_LOW);
            }

        Led (Pio *pio, bool active_high=true)
            {
                _pio = pio;
                _polarity = active_high ? ActiveHigh : ActiveLow;
                _pio->setMode (_polarity == ActiveHigh ? PIO_OUTPUT_HIGH : PIO_OUTPUT_LOW);
            }                        

        // Set the LED state.
        void write (bool state)
            {
                if (_polarity == ActiveLow)
                    state = !state;
                *_pio = state;                
            }
        
        // Toggle the LED state.        
        void toggle ()
            {
                _pio->toggle ();
            }
        
        // Set the LED state.
        void operator = (bool state)
            {
                write (state);
            }    
    };

}

#endif
