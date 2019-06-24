#include "Spi.hpp"

namespace Wacky
{

    bool Spi::begin (pio_t cs_pio, spi_mode_t mode,
                     spi_cs_mode_t cs_mode,
                     spi_clock_speed_t clock_speed_kHz, uint8_t bits)
    {
        spi_cfg_t spi_cfg =
        {
            // Could be smarter about channel allocation if wanted
            // automatic chip select driving.
            .channel = 0,
            .clock_speed_kHz = clock_speed_kHz,
            .cs = cs_pio,
            .mode = mode,
            .cs_mode = cs_mode,
            .bits = bits
        };

        _spi = spi_init (&spi_cfg);
        if (! _spi)
            return 0;
        
        return 1;
    }
    

    spi_ret_t Spi::transact (spi_transfer_t *transfer, uint8_t size)
    {
        return spi_transact (_spi, transfer, size);
    }


    spi_ret_t Spi::transfer (const void *txbuffer, void *rxbuffer, 
                             spi_size_t len, bool terminate)
    {
        return spi_transfer (_spi, txbuffer, rxbuffer, len, terminate);
    }
    

    void Spi::shutdown ()
    {
        spi_shutdown (_spi);
    }


    void Spi::end ()
    {
        spi_shutdown (_spi);
    }

}
