#ifndef MPU9250_HPP
#define MPU9250_HPP

#include "Twi.hpp"

// The MPU9250 contains an IMU (3-axis accelerometer and 3-axis
// gyroscope) and a 3-axis magnetometer.  The IMU responds at two
// different I2C addresses depending on the state of the AD0 pin.  The
// magnetometer has a different I2C address but the IMU needs to be
// configured first so that commands can be passed through to the
// magnetometer.

namespace Wacky
{

    class Mpu9250
    {
    public:
        
        enum mpu_address {MPU_0, MPU_1};    
        
        int begin (mpu_address channel=MPU_0,
                   Wacky::Twi::channel twi_channel=Wacky::Twi::TWI_0);

        // Return true if IMU has data ready.
        bool isImuReady ();

        // Return raw accelerometer data.
        bool readAccel (int16_t acceldata[3]);

        // Return raw gyroscope data.        
        bool readGyro (int16_t gyrodata[3]);

        // Return true if magnetometer has data ready.
        bool isMagReady ();

        // Return raw magetometer data.        
        bool readMag (int16_t magdata[3]);
        
    private:
        Wacky::Twi _twi;
        uint8_t _imu_slave_addr;
        uint8_t _mag_slave_addr;
        
        bool initImu ();
        bool readImuData (const uint8_t addr, int16_t data[3]);
        uint8_t readImuByte (const uint8_t addr);    
        uint8_t writeImuByte (const uint8_t addr, const uint8_t value);
        bool initMag();
        uint8_t readMagByte (const uint8_t addr);
        uint8_t writeMagByte (const uint8_t addr, const uint8_t value);
        uint8_t readByte (const uint8_t slave_addr, const uint8_t addr);
        uint8_t writeByte (const uint8_t slave_addr, const uint8_t addr,
                           const uint8_t value);
    };

}

#endif
