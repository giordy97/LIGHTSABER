#ifndef MPU6050_H
#define MPU6050_H
#include <Arduino.h>

extern void init_mpu6050();
extern void MPU_Get_Data(int16_t* GyX,int16_t* GyZ);
void MPU_Get_Data_Clash(int16_t* AcX,int16_t* AcY,int16_t* AcZ,int16_t* GyX,int16_t* GyZ);
void MPU_Get_Rotation(int16_t* GyY);

#endif //MPU6050_H
