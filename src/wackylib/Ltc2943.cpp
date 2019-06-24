#include "bits.h"
#include "Ltc2943.hpp"

namespace Wacky
{

#define LTC2943_SLAVE_ADDR 0x64

#define LTC2943_STATUS 0
#define LTC2943_CONTROL 1
#define LTC2943_CHARGE_MSB 2
#define LTC2943_CHARGE_LSB 3
#define LTC2943_CHARGE_THRESHOLD_HIGH_MSB 4
#define LTC2943_CHARGE_THRESHOLD_HIGH_LSB 5
#define LTC2943_CHARGE_THRESHOLD_LOW_MSB 6
#define LTC2943_CHARGE_THRESHOLD_LOW_LSB 7
#define LTC2943_VOLTAGE_MSB 8
#define LTC2943_VOLTAGE_LSB 9
#define LTC2943_VOLTAGE_THRESHOLD_HIGH_MSB 10
#define LTC2943_VOLTAGE_THRESHOLD_HIGH_LSB 11
#define LTC2943_VOLTAGE_THRESHOLD_LOW_MSB 12
#define LTC2943_VOLTAGE_THRESHOLD_LOW_LSB 13
#define LTC2943_CURRENT_MSB 14
#define LTC2943_CURRENT_LSB 15
#define LTC2943_CURRENT_THRESHOLD_HIGH_MSB 16
#define LTC2943_CURRENT_THRESHOLD_HIGH_LSB 17
#define LTC2943_CURRENT_THRESHOLD_LOW_MSB 18
#define LTC2943_CURRENT_THRESHOLD_LOW_LSB 19
#define LTC2943_TEMP_MSB 20
#define LTC2943_TEMP_LSB 21
#define LTC2943_TEMP_THRESHOLD_HIGH 22
#define LTC2943_TEMP_THRESHOLD_LOW 23


uint8_t Ltc2943::readByte (const uint8_t addr)
{
    uint8_t response = 0;
    uint8_t status;
    
    status = _twi.read (LTC2943_SLAVE_ADDR,
                        addr, 1, &response, 1);
    return response;
}

uint8_t Ltc2943::writeByte (const uint8_t addr, const uint8_t value)
{
    return _twi.write (LTC2943_SLAVE_ADDR, addr, 1, &value, 1);
}    


uint16_t Ltc2943::readWord (const uint8_t addr)
{
    uint8_t msb;
    uint8_t lsb;
    
    msb = readByte (addr);
    lsb = readByte (addr + 1);
    return msb << 8 | lsb;
}


uint16_t Ltc2943::writeWord (const uint8_t addr, uint16_t value)
{
    writeByte (addr, value >> 8);
    return writeByte (addr + 1, value &0xFF);
}


Ltc2943::status Ltc2943::begin ()
{
    if (! _twi.begin ())
        return NO_DEVICE;
    
    // Set scan mode for ADC conversions and prescale of 4096
    writeByte (LTC2943_CONTROL, 0xB8);
    if (readByte (LTC2943_CONTROL) != 0xB8)
        return NO_DEVICE;                    
    
    return OK;
}


uint16_t Ltc2943::readCharge ()
{
    return readWord (LTC2943_CHARGE_MSB);
}


uint16_t Ltc2943::readVoltage ()
{
    return readWord (LTC2943_VOLTAGE_MSB);
}


uint16_t Ltc2943::readCurrent ()
{
    return readWord (LTC2943_CURRENT_MSB);
}    


uint16_t Ltc2943::readTemperature ()
{
    return readWord (LTC2943_TEMP_MSB);
}


uint16_t Ltc2943::getPrescale ()
{
    uint8_t control;
    uint16_t prescale;

    control = readByte (LTC2943_CONTROL);
    prescale = 1 << (BITS_EXTRACT (control, 3, 5) * 2);
    if (prescale > 4096)
        prescale = 4096;
    return prescale;
}


void Ltc2943::setPrescale (uint16_t prescale)
{
    int i;
    uint8_t control;    
    uint16_t prescale1 = 1;
    
    for (i = 0; i < 7; i++)
    {
        if (prescale1 <= prescale)
            break;
        prescale1 << 2; 
    }

    control = readByte (LTC2943_CONTROL);
    BITS_INSERT (control, i, 3, 5);
    writeByte (LTC2943_CONTROL, control);
}


void Ltc2943::shutdown ()
{
    writeByte (LTC2943_CONTROL, 0X01);
}

}
