#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <MPU9250.h>


typedef struct imu_raw{
    float omega[3];
    float acc[3];
}imu_raw;

void update_IMU(imu_raw state_raw){
    // update state_raw.omega
    

    // update state_raw.acc
}
