#include "twi.h"

typedef struct
{
    twi_t twi;
} mpu9250_t;


bool
mpu9250_imu_data_ready_p (mpu9250_t *mpu9250);

bool
mpu9250_imu_accel_read (mpu9250_t *mpu9250, int16_t acceldata[3]);

bool
mpu9250_imu_gyro_read (mpu9250_t *mpu9250, int16_t gyrodata[3]);

bool
mpu9250_mag_data_ready_p (mpu9250_t *mpu9250);

bool
mpu9250_mag_data_read (mpu9250_t *mpu9250, int16_t magdata[3]);


mpu9250_t *
mpu9250_init (void);
