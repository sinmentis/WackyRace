#ifndef LTC2943_HPP
#define LTC2943_HPP

#include "Twi.hpp"

namespace Wacky
{

class Ltc2943
{
public:
    
    enum status {OK = 1, NO_DEVICE = -1};
    
    status begin();

    // Return raw 16-bit accumulated charge.
    uint16_t readCharge ();

    // Return raw 16-bit voltage.    
    uint16_t readVoltage ();

    // Return raw 16-bit current.
    uint16_t readCurrent ();

    // Return raw 16-bit temperature.        
    uint16_t readTemperature ();

    // Get prescale value for accumulated charge.
    uint16_t getPrescale ();

    // Set prescale value for accumulated charge.    
    void setPrescale (uint16_t prescale);
    
    void shutdown();

    // could have calcCharge, calcVoltage, methods etc.

    
private:
    Wacky::Twi _twi;
    
    uint8_t readByte (const uint8_t addr);
    
    uint8_t writeByte (const uint8_t addr, uint8_t value);
    
    uint16_t readWord (const uint8_t addr);
    
    uint16_t writeWord (const uint8_t addr, uint16_t value);
};

}

#endif
