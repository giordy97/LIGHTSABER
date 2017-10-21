#include "Mpu6050.h"
#include "Config.h"
#include <Wire.h>

void init_mpu6050(){
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  delay(500);
}

void MPU_Get_Data(int16_t* GyX,int16_t* GyZ){
  Wire.beginTransmission(MPU_addr);                         //CONTROLLO SUI MOVIMENTI
  Wire.write(0x43);                                         // starting with register 0x43 (GYRO_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,6,true);                        // request a total of 6 registers  
  (*GyX) = Wire.read()<<8|Wire.read();                      // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  Wire.read();Wire.read();                                  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  (*GyZ) = Wire.read()<<8|Wire.read();                      // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
}

void MPU_Get_Data_Clash(int16_t* AcX,int16_t* AcY,int16_t* AcZ,int16_t* GyX,int16_t* GyZ){
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);                     // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true);   // request a total of 14 registers
  (*AcX) = Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
  (*AcY) = Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  (*AcZ) = Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
           Wire.read();   Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  (*GyX) = Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
           Wire.read();   Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  (*GyZ) = Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
}

void MPU_Get_Rotation(int16_t* GyY){
  Wire.beginTransmission(MPU_addr);                         //CONTROLLO SUI MOVIMENTI
  Wire.write(0x45);                                         // starting with register 0x43 (GYRO_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,2,true);                        // request a total of 6 registers  
  (*GyY) = Wire.read()<<8|Wire.read();                      // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
}
