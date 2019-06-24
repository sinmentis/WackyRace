#ifndef PIO_HPP
#define PIO_HPP

#include "pio.h"


namespace Wacky
{

    class Pio
    {
    public:
        Pio (pio_t pio) {_pio = pio;};

        /** Set mode of the PIO pin.  This is either:
            PIO_INPUT
            PIO_PULLUP  (input with pull-up resistor enabled)
            PIO_PULLDOWN (input with pull-down resistor enabled)
            PIO_OUTPUT_LOW (output, initially low)
            PIO_OUTPUT_HIGH (output, initially high)
            PIO_PERIPH_A (peripheral function A)
            PIO_PERIPH_A_PULLUP (peripheral function A with pull-up)
            PIO_PERIPH_B (peripheral function B)
            PIO_PERIPH_B_PULLUP (peripheral function B with pull-up)
            PIO_PERIPH_C (peripheral function C)
            PIO_PERIPH_C_PULLUP (peripheral function C with pull-up)
        */
        void setMode (pio_config_t mode)
            {
                pio_config_set (_pio, mode);
            }

        /** Toggle output state.  */
        void toggle ()
            {
                pio_output_toggle (_pio);
            }                

        /** Set output state.  */        
        void write (bool state)
            {
                pio_output_set (_pio, state);
            }

        /** Read input state.  */                
        bool read ()
            {
                return pio_input_get (_pio);
            }

        /** Enable port change interrupt.  */                        
        void enableIrq ()
            {
                pio_irq_enable (_pio);
            }

        /** Disable port change interrupt.  */                                
        void disableIrq ()
            {
                pio_irq_disable (_pio);
            }                        

        /* Switch off PIO clocks to save power.  */
        void shutdown ()
            {
                pio_shutdown (_pio);
            }                        

        /** Set the pin state with operating overloading, for example,

            Pio pin (PA0_PIO);

            pin.modeSet (PIO_OUTPUT_LOW);
            pin = 1;
         */
        void operator = (bool state)
            {
                pio_output_set (_pio, state);
            }

        /* Get the pin state, for example,

            Pio pin (PA0_PIO);

            pin.modeSet (PIO_INPUT_PULLUP);

            if (pin)
                do-something;
        */
        operator bool() const {return pio_input_get(_pio);}
        
    private:
        pio_t _pio;
    };

}

#endif
