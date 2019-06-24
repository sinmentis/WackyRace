#ifndef SPI_HPP
#define SPI_HPP

#include "pio.h"
#include "spi.h"

namespace Wacky
{

    class Spi
    {
    public:

        bool begin (pio_t cs_pio,  spi_mode_t mode = SPI_MODE_0,
                    spi_cs_mode_t cs_mode = SPI_CS_MODE_FRAME,
                    spi_clock_speed_t clock_speed_kHz = 1000, uint8_t bits = 8);


        /** Transfer a sequence of bytes to/from SPI using multiple buffers.
            In chip select framing mode, this will drive the CS low
            for the whole transaction.
            @param spi SPI channel to use.
            @param transfer Array of transmit/receive buffers and sizes.  
            @param size Size of transfer array.
            @return Number of bytes transferred.
        */        
        spi_ret_t transact (spi_transfer_t *transfer, uint8_t size);


        /** Transfer a sequence of bytes to/from SPI.
            @param spi SPI channel to use.
            @param txbuffer Data buffer to write from (or NULL just for reading).
            @param rxbuffer Data buffer to read into (or NULL just for writing).
            @param len Number of bytes to transfer.
            @param terminate True to negate CS when last byte transferred.
            @return Number of bytes transferred.
        */
        spi_ret_t transfer (const void *txbuffer, void *rxbuffer, 
                            spi_size_t len, bool terminate);
        
        void shutdown ();

        void end ();
        
    private:
        spi_t _spi;
    };
}

#endif
