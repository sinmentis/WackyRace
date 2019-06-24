#include "Twi.hpp"

namespace Wacky
{
    Twi::Twi ()
        : _read_timeout (1000), _write_timeout (1000)        
    {
    }
    
    
    bool Twi::begin (channel channel, uint16_t clock_speed_kHz)
    {
        twi_cfg_t twi_cfg =
            {
                .channel = channel,
                .period = TWI_PERIOD_DIVISOR (clock_speed_kHz * 1000)
            };
        
        _twi = twi_init (&twi_cfg);
        if (! _twi)
            return 0;
        
        return 1;
    }


    twi_ret_t Twi::write (twi_slave_addr_t slave_addr,
                          twi_iaddr_t addr, uint8_t addr_size,
                          const void *buffer, twi_size_t size)
    {

       
        return twi_master_addr_write_timeout (_twi, slave_addr,
                                              addr, addr_size,
                                              buffer, size,
                                              _write_timeout);
    }


    twi_ret_t Twi::read (twi_slave_addr_t slave_addr,
                         twi_iaddr_t addr, uint8_t addr_size,
                         void *buffer, twi_size_t size)
    {

       
        return twi_master_addr_read_timeout (_twi, slave_addr,
                                              addr, addr_size,
                                              buffer, size,
                                              _read_timeout);
    }    

    
    void Twi::shutdown ()
    {
        twi_shutdown (_twi);
    }


    void Twi::end ()
    {
        twi_shutdown (_twi);
    }

}
