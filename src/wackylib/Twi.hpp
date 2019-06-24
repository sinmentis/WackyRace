#ifndef TWI_HPP
#define TWI_HPP

#include "pio.h"
#include "twi.h"

namespace Wacky
{

    class Twi
    {
    public:

        enum channel {TWI_0, TWI_1};    
        
        Twi ();
        
        bool begin (channel channel = TWI_0,
                    uint16_t clock_speed_kHz = 100);

        twi_ret_t write (twi_slave_addr_t slave_addr,
                         twi_iaddr_t addr, uint8_t addr_size,
                         const void *buffer, twi_size_t size);

        twi_ret_t read (twi_slave_addr_t slave_addr,
                        twi_iaddr_t addr, uint8_t addr_size,
                        void *buffer, twi_size_t size);        
        
        void shutdown ();

        void end ();
        
    private:
        twi_t _twi;
        twi_timeout_t _read_timeout;
        twi_timeout_t _write_timeout;        
    };
}

#endif
