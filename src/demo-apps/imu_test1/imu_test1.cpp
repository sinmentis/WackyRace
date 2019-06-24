/* File:   imu_test1.c
   Author: M. P. Hayes, UCECE
   Date:   23 Feb 2018
   Descr: 
*/
#include "UsbSerial.hpp"
#include "Mpu9250.hpp"

int main (void)
{
    Wacky::UsbSerial usbserial;
    Wacky::Mpu9250 imu;
    int16_t acceldata[3];    

    usbserial.begin ();
    
    while (! imu.begin (Wacky::Mpu9250::MPU_0))
        usbserial.println ("Cannot detect IMU");
    
    while (1)
    {
        imu.readAccel (acceldata);        
        
        usbserial.print (acceldata[0]);
        usbserial.print (" ");        
        usbserial.print (acceldata[1]);
        usbserial.print (" ");                
        usbserial.print (acceldata[2]);
        usbserial.print (" ");
        usbserial.println ();            
    }
}
