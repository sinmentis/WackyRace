#include "Pio.hpp"
#include "Spi.hpp"

namespace Wacky
{

    class Nrf24
    {
    public:

        enum data_rate {RF24_1MBPS = 0, RF24_2MBPS, RF24_250KBPS};

        enum crc_length {RF24_CRC_DISABLED = 0, RF24_CRC_8, RF24_CRC_16};

        enum status {OK = 1, NO_RADIO = -1};

        Nrf24 ();

        // Select channel, address, and payload size.  The maximum
        // payload size is 32 bytes.
        status begin (uint8_t channel, uint64_t address, uint8_t payload_size=32);

        // Switch to receiver mode and listen on configured channel
        // and address.
        void listen ();        

        // Read received data (up to max of 32 bytes).  Returns number
        // of bytes read or 0 if not data available.
        uint8_t read (void *buffer, uint8_t len);

        // Switch to transmit mode and write buffer (up to max of 32
        // bytes).  Returns number of bytes written.  This can block
        // for about 5 ms all going well and 15 ms if there is no response.
        uint8_t write (const void *buffer, uint8_t len);        

        // Return true if receiver has data ready.
        bool isDataReady ();

        // Set channel number 0--127.
        void setChannel (uint8_t channel);

        // Set address.  By default, only the 5 least significant bytes used.
        void setAddress (uint64_t address);

        // Set the address size in bytes (3--5).
        void setAddressSize (uint8_t size);

        void powerDown ();

        void powerUp ();

        // Set power amplifier level (0--3).  
        void setPaLevel (uint8_t level);

        bool setDataRate (data_rate speed);

        void setCrcLength (crc_length length);

        // The retransmit delay is in steps of 250 us.   count is the number
        // of retries.   The default count is the maximum value 15.
        void setRetries (uint8_t delay, uint8_t count);

        // Enable/disable auto acknowledge.  Auto acknowledge is enabled by default.
        void setAutoAck (bool state);

        void setDynamicPayloads (bool state);

    private:
        Wacky::Pio _ce_pio;
#ifdef RADIO_IRQ_PIO
        Wacky::Pio _irq_pio;
#endif
        Wacky::Spi _spi;
        uint8_t _address_bytes[5];
        bool _wide_band;
        bool _p_variant;
        uint8_t _payload_size;
        uint8_t _ack_payload_length;
        bool _dynamic_payloads_enabled;
        bool _listening;

        uint8_t readRegisters (uint8_t reg, uint8_t *buffer, uint8_t len);

        uint8_t readRegister (uint8_t reg);

        uint8_t writeRegisters (uint8_t reg, const uint8_t *buffer,
                                uint8_t len);

        uint8_t writeRegister (uint8_t reg, uint8_t value);

        uint8_t writeCommand (uint8_t command);

        uint8_t getStatus ();        

        void flushRx ();

        void flushTx ();

        void setTxAddr (uint8_t *address_bytes);

        void setRx0Addr (uint8_t *address_bytes);

        void makeAddr (uint64_t address, uint8_t *address_bytes);
    };
}

