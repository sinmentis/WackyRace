/* TheM nrf24l01+ has six pipes for reading.  Each pipe matches on a
   different address and thus one receiver can communicate with six
   transmitters.   Each pipe shares the same frequency channel.

   Each pipe can have up to a five byte configurable address.  Pipe 0
   has a unique five byte address but pipes 1--5 share the four MS
   bytes.  The LS byte must be unique for all pipes.

   When a message is transmitted, it is prefixed by an address.

   To ensure that the ACK packet from the PRX is transmitted to the
   correct PTX, the PRX takes the data pipe address where it received
   the packet and uses it as the TX address when transmitting the ACK
   packet.

   On the PTX the TX_ADDR must be the same as RX_ADDR_P0.
   
*/


#include "Nrf24.hpp"
#include "Spi.hpp"
#include "delay.h"


namespace Wacky
{

#define TPD_TO_STDBY 3

#define WRITE_STATUS_US 100
#define WRITE_TIMEOUT_US 10000
#define WRITE_TIMEOUTS (WRITE_TIMEOUT_US / WRITE_STATUS_US)

// Registers
#define CONFIG      0x00
#define EN_AA       0x01
#define EN_RXADDR   0x02
#define SETUP_AW    0x03
#define SETUP_RETR  0x04
#define RF_CH       0x05
#define RF_SETUP    0x06
#define STATUS      0x07
#define OBSERVE_TX  0x08
#define CD          0x09
#define RX_ADDR_P0  0x0A
#define RX_ADDR_P1  0x0B
#define RX_ADDR_P2  0x0C
#define RX_ADDR_P3  0x0D
#define RX_ADDR_P4  0x0E
#define RX_ADDR_P5  0x0F
#define TX_ADDR     0x10
#define RX_PW_P0    0x11
#define RX_PW_P1    0x12
#define RX_PW_P2    0x13
#define RX_PW_P3    0x14
#define RX_PW_P4    0x15
#define RX_PW_P5    0x16
#define FIFO_STATUS 0x17
#define DYNPD	    0x1C
#define FEATURE	    0x1D

// Bits
#define MASK_RX_DR  6
#define MASK_TX_DS  5
#define MASK_MAX_RT 4
#define EN_CRC      3
#define CRCO        2
#define PWR_UP      1
#define PRIM_RX     0
#define ENAA_P5     5
#define ENAA_P4     4
#define ENAA_P3     3
#define ENAA_P2     2
#define ENAA_P1     1
#define ENAA_P0     0
#define ERX_P5      5
#define ERX_P4      4
#define ERX_P3      3
#define ERX_P2      2
#define ERX_P1      1
#define ERX_P0      0
#define AW          0
#define ARD         4
#define ARC         0
#define PLL_LOCK    4
#define RF_DR       3
#define RF_PWR      6
#define RX_DR       6
#define TX_DS       5
#define MAX_RT      4
#define RX_P_NO     1
#define TX_FULL     0
#define PLOS_CNT    4
#define ARC_CNT     0
#define TX_REUSE    6
#define FIFO_FULL   5
#define TX_EMPTY    4
#define RX_FULL     1
#define RX_EMPTY    0
#define DPL_P5	    5
#define DPL_P4	    4
#define DPL_P3	    3
#define DPL_P2	    2
#define DPL_P1	    1
#define DPL_P0	    0
#define EN_DPL	    2
#define EN_ACK_PAY  1
#define EN_DYN_ACK  0

// Commands
#define R_REGISTER          0x00
#define W_REGISTER          0x20
#define REGISTER_MASK       0x1F
#define ACTIVATE            0x50
#define R_RX_PL_WID         0x60
#define R_RX_PAYLOAD        0x61
#define W_TX_PAYLOAD        0xA0
#define W_ACK_PAYLOAD       0xA8
#define FLUSH_TX            0xE1
#define FLUSH_RX            0xE2
#define REUSE_TX_PL         0xE3
#define W_TX_PAYLOAD_NOACK  0xB0
#define NOP                 0xFF


#define RF_DR_LOW   5
#define RF_DR_HIGH  3
#define RF_PWR_LOW  1
#define RF_PWR_HIGH 2
    
    Nrf24::Nrf24 ()
    : _ce_pio (RADIO_CE_PIO)
#ifdef RADIO_IRQ_PIO
    , _irq_pio (RADIO_IRQ_PIO)
#endif
    {
    }
      
      
      Nrf24::status Nrf24::begin (uint8_t channel, uint64_t address, uint8_t payload_size)
      {
        _spi.begin (RADIO_CS_PIO, SPI_MODE_0, SPI_CS_MODE_FRAME, 4000, 8);

        _ce_pio.setMode (PIO_OUTPUT_LOW);

#ifdef RADIO_IRQ_PIO
        _irq_pio.setMode (PIO_INPUT);
#endif

        setAddress (address);

        _wide_band = true;
        _p_variant = false;
        _payload_size = payload_size;
        _ack_payload_length = 0;
        _listening = false;
        
        delay_ms (5);

        writeRegister (RX_PW_P0, _payload_size);
        
        writeRegister (CONFIG, BIT (EN_CRC));

        uint8_t config = readRegister (CONFIG);
        if (config != BIT (EN_CRC))
            return NO_RADIO;

        // Set the maximum number of retransmissions (15).
        setRetries (5, 5);

        setPaLevel (3);

        if (setDataRate (RF24_250KBPS))
            _p_variant = true;
        
        // Then set the data rate to the slowest (and most reliable)
        // speed supported by all hardware.
        setDataRate (RF24_1MBPS);
    
        // Initialize CRC and request 2-byte (16bit) CRC
        setCrcLength (RF24_CRC_16);

        setDynamicPayloads (false);
    
        // Clear current status bits
        writeRegister (STATUS, BIT (RX_DR) | BIT (TX_DS) | BIT (MAX_RT));
    
        setChannel (channel);
    
        // Flush buffers
        flushRx ();
        flushTx ();

        return OK;
    }

    
    uint8_t Nrf24::readRegisters (uint8_t reg, uint8_t *buffer, uint8_t len)
    {
        uint8_t status;
        uint8_t reg_address = R_REGISTER | (REGISTER_MASK & reg);
        spi_transfer_t transfers[] =
            {
                // As the register address is sent, the status is received.
                {&reg_address, &status, 1},
                {0, buffer, len}
            };

        _spi.transact (transfers, 2);
        return status;
    }
    
    uint8_t Nrf24::writeRegisters (uint8_t reg, const uint8_t *buffer,
                                    uint8_t len)
    {
        uint8_t status;
        uint8_t reg_address = W_REGISTER | (REGISTER_MASK & reg);
        spi_transfer_t transfers[] =
            {
                // As the register address is sent, the status is received.
                {&reg_address, &status, 1},
                {buffer, 0, len}
            };        
        
        _spi.transact (transfers, 2);
        return status;
    }

    uint8_t Nrf24::readRegister (uint8_t reg)
    {
        uint8_t result;

        readRegisters (reg, &result, 1);
        return result;
    }
    
    uint8_t Nrf24::writeRegister (uint8_t reg, uint8_t value)
    {
        return writeRegisters (reg, &value, 1);
    }

    uint8_t Nrf24::writeCommand (uint8_t command)
    {
        uint8_t status;
        
        _spi.transfer (&command, &status, 1, true);
        return status;
    }

    bool Nrf24::isDataReady ()
    {
        uint8_t status;
        
#ifdef RADIO_IRQ_PIO
        // Poll IRQ pin (active low) to see if receiver has data.
        // This is less noisy than polling over SPI.
        if (_irq_pio.read ())
            return false;
#endif
        
        // Poll status register to see if receiver ready.
        status = getStatus ();
        if (! (status & BIT (RX_DR)))
            return false;

        // Could determine which pipe is ready from status.
        return true;
    }
    
    uint8_t Nrf24::read (void *buffer, uint8_t len)
    {
        uint8_t status;
        uint8_t blank_len;
        uint8_t reg_address = R_RX_PAYLOAD;
        uint8_t *data = (uint8_t *)buffer;

        if (! isDataReady ())
            return 0;
        
        if (len > _payload_size)
            len = _payload_size;
        blank_len = _dynamic_payloads_enabled ? 0 : _payload_size - len;

        spi_transfer_t transfers[] =
            {
                {&reg_address, &status, 1},
                {0, buffer, len},
                {0, 0, blank_len},                
            };

        _spi.transact (transfers, blank_len == 0 ? 2 : 3);        
        
        // Clear the status RX_DR bit
        writeRegister (STATUS, BIT (RX_DR));

        // Handle ack payload receipt
        if (status & BIT (TX_DS))
            writeRegister (STATUS, BIT (TX_DS));

        // Check for end of transmission.
        if (! readRegister (FIFO_STATUS) & BIT (RX_EMPTY))
            return 0;

        return _payload_size;
    }

    void Nrf24::flushRx ()
    {
        writeCommand (FLUSH_RX);
    }

    void Nrf24::flushTx ()
    {
        writeCommand (FLUSH_TX);        
    }

    uint8_t Nrf24::getStatus ()
    {
        return writeCommand (NOP);
    }


    void Nrf24::setAddress (uint64_t address)
    {
        makeAddr (address, _address_bytes);
    }

    
    void Nrf24::setChannel (uint8_t channel)
    {
        if (channel > 127)
            channel = 127;

        writeRegister (RF_CH, channel);
    }

    void Nrf24::listen ()
    {
        // TODO: allow other pipes with different signature
        setRx0Addr (_address_bytes);

        writeRegister (EN_RXADDR,
                        readRegister (EN_RXADDR) | BIT (ERX_P0));
        
        writeRegister (CONFIG,
                        readRegister (CONFIG) | BIT (PWR_UP) | BIT (PRIM_RX));
        
        writeRegister (STATUS, BIT (RX_DR) | BIT (TX_DS) | BIT (MAX_RT));
        
        flushRx ();
        flushTx ();
        _ce_pio.write (1);
        _listening = true;        

        // Wait for the radio to come up
        delay_ms (TPD_TO_STDBY);
    }
    

    void Nrf24::powerDown ()
    {
        writeRegister (CONFIG, readRegister (CONFIG) & ~BIT (PWR_UP));
    }
    

    void Nrf24::powerUp ()
    {
        writeRegister (CONFIG, readRegister (CONFIG) | BIT (PWR_UP));
    }


    void Nrf24::setTxAddr (uint8_t *address_bytes)
    {
        writeRegisters (TX_ADDR, address_bytes, 5);
    }


    void Nrf24::setRx0Addr (uint8_t *address_bytes)
    {
        writeRegisters (RX_ADDR_P0, address_bytes, 5);
    }    

    
    void Nrf24::makeAddr (uint64_t address, uint8_t *address_bytes)
    {
        int i;

        for (i = 0; i < 5; i++)
        {
            *address_bytes++ = address & 0xff;
            address >>= 8;
        }
    }        
    
    
    uint8_t Nrf24::write (const void *buffer, uint8_t len)
    {
        uint8_t status;
        volatile uint32_t timeouts = 0;
        uint8_t reg_address = W_TX_PAYLOAD;
        uint8_t blank_len;
        uint8_t blank_tx = 0xff;
        uint8_t *data = (uint8_t *)buffer;        

        if (_listening)
        {
            // Switch out of listening mode.
            _ce_pio.write (0);            
            flushTx ();
            flushRx ();
            _listening = false;
        }
        
        // There is no need to continually update the RX and TX
        // addresses but this is a minor optimisation.
        setTxAddr (_address_bytes);
        setRx0Addr (_address_bytes);
        
        // Power up transmitter and set PRIM low.
        writeRegister (CONFIG,
                        (readRegister (CONFIG) | BIT (PWR_UP)) & ~BIT (PRIM_RX));
        delay_ms (TPD_TO_STDBY);

        if (len > _payload_size)
            len = _payload_size;
        blank_len = _dynamic_payloads_enabled ? 0 : _payload_size - len;
    
        spi_transfer_t transfers[] =
            {
                {&reg_address, &status, 1},
                {buffer, 0, len},
                {0, 0, blank_len},                
            };

        _spi.transact (transfers, blank_len == 0 ? 2 : 3);

        // A high pulse (10 us min) starts transmission.
        _ce_pio.write (1);                    
        DELAY_US (15);
        _ce_pio.write (0);                            

        // If auto acknowledge enabled, the radio switches to RX mode
        // after the data packet has been transmitted.
        
        // Poll for the TX_DS bit (an auto acknowledge is received) or
        // the MAX_RT bit (the maximum number of retransmits has
        // occurred).
        timeouts = WRITE_TIMEOUTS;
        while (timeouts--)
        {
            uint8_t observe_tx;

            // Read status and observe_tx.  The latter tracks the
            // number of lost and retransmitted packets for debugging.
            status = readRegisters (OBSERVE_TX, &observe_tx, 1);
            if (status & (BIT (TX_DS) | BIT (MAX_RT)))
                break;

            DELAY_US (WRITE_STATUS_US);
        }

        // Read status and then clear status bits.
        status = writeRegister (STATUS, BIT (RX_DR) | BIT (TX_DS) | BIT (MAX_RT));

        // Auto acknowledge can send an ack packet.
        if (status & BIT (RX_DR))
        {
            // TODO.  This is not currently supported.
            _ack_payload_length = 0;
        }

        // Clear the transmit-complete bit.
        writeRegister (STATUS, readRegister (STATUS) | BIT (TX_DS));

        // Flush transmitter buffer. 
        flushTx ();

        // If had no auto acknowledge, then cannot guarantee that the
        // transmitted data packet was received.
        if (! (status & BIT (TX_DS)))
            return 0;

        // Return number of bytes transmitted.
        return _payload_size;
    }
    
    
    void Nrf24::setPaLevel (uint8_t level)
    {
        uint8_t setup = readRegister (RF_SETUP);

        setup &= ~(BIT (RF_PWR_LOW) | BIT (RF_PWR_HIGH));

        switch (level)
        {
        default:
        case 3:
            setup |= BIT (RF_PWR_LOW) | BIT (RF_PWR_HIGH);
            break;
        case 2:
            setup |= BIT (RF_PWR_HIGH);
            break;
        case 1:
            setup |= BIT (RF_PWR_LOW);
        case 0:
            break;
        }
        writeRegister (RF_SETUP, setup);
    }
    
    bool Nrf24::setDataRate (data_rate speed)
    {
        bool result = false;
        uint8_t setup = readRegister (RF_SETUP);
        
        // HIGH and LOW '00' is 1Mbs - our default
        _wide_band = false;
        setup &= ~ (BIT (RF_DR_LOW) | BIT (RF_DR_HIGH));
        if (speed == RF24_250KBPS)
        {
            // Must set the RF_DR_LOW to 1; RF_DR_HIGH (used to be RF_DR) is already 0
            // Making it '10'.
            _wide_band = false;
            setup |= BIT (RF_DR_LOW);
        }
        else
        {
            // Set 2Mbs, RF_DR (RF_DR_HIGH) is set 1
            // Making it '01'
            if (speed == RF24_2MBPS)
            {
                _wide_band = true;
                setup |= BIT (RF_DR_HIGH);
            }
            else
            {
                // 1Mbs
                _wide_band = false;
            }
        }
        writeRegister (RF_SETUP, setup);
        
        // Verify our result
        if (readRegister (RF_SETUP) == setup)
        {
            result = true;
        }
        else
        {
            _wide_band = false;
        }
        
        return result;
    }
    
    void Nrf24::setCrcLength (crc_length length)
    {
        uint8_t config;

        config = readRegister (CONFIG) & ~ (BIT (CRCO) | BIT (EN_CRC));

        switch (length)
        {
        case RF24_CRC_DISABLED:
            break;

        case RF24_CRC_8:
            config |= BIT (EN_CRC);
            break;

        case RF24_CRC_16:            
            config |= BIT (EN_CRC) | BIT (CRCO);
            break;
        }
        writeRegister (CONFIG, config);
    }

    // The retransmit delay is in steps of 250 us.   count is the number
    // of retries.
    void Nrf24::setRetries (uint8_t delay, uint8_t count)
    {
        if (delay > 15)
            delay = 15;
        if (count > 15)
            count = 15;
        
        writeRegister (SETUP_RETR, (delay & 0xf) << ARD | (count & 0xf) << ARC);
    }
    
    void Nrf24::setAutoAck (bool state)
    {
        // This can be controlled for each pipe.
        if (state)
            writeRegister (EN_AA, 0x7f);
        else
            writeRegister (EN_AA, 0x00);            
    }
    
    void Nrf24::setAddressSize (uint8_t size)
    {
        if (size == 0)
        {
            writeRegister (SETUP_AW, size);
            return;
        }

        if (size > 5)
            size = 5;
        else if (size < 3)
            size = 3;
        
        writeRegister (SETUP_AW, size - 2);
    }

    void Nrf24::setDynamicPayloads (bool state)
    {
        writeRegister (DYNPD, state);
        _dynamic_payloads_enabled = state;
    }
}
