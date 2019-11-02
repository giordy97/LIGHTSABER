#ifndef MPU6050_H
#define MPU6050_H
#include <Arduino.h>
#include "Config.h"
#include <Wire.h>

extern void init_mpu6050();
extern void MPU_Get_Data(int32_t* AcX,int32_t* AcZ,int32_t* GyX,int32_t* GyZ);
extern void MPU_Get_Acc(int32_t* AcX,int32_t* AcZ);
extern void MPU_Get_Rotation(int16_t* GyY);

#endif //MPU6050_H
