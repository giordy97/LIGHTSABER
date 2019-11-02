#include<Wire.h>
#define gyro_force   750000000
#define clash_force 1100000000
#define MPU_addr 0x68
int32_t AcX,AcY,AcZ,GyX,GyY,GyZ;
bool c = false;

void setup(){
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  Serial.begin(9600);
}

void loop(){
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true);  // request a total of 14 registers
  AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)     
  AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
      Wire.read();   Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)

  if(((GyX*GyX) + (GyZ*GyZ))  > gyro_force) Serial.print("\n\n\t\t\tSWING GYRO  : ");Serial.print((GyX*GyX) + (GyZ*GyZ)); Serial.print("\t\t\t\n");
  
  if(((AcX*AcX) + (AcZ*AcZ))  > clash_force) Serial.print("\n\n\t\t\tCLASH ACC   : ");Serial.print((AcX*AcX) + (AcZ*AcZ)); Serial.print("\t\t\t\n");
   
  Serial.print("AcX= "); Serial.print(AcX);
  Serial.print(" | AcY= "); Serial.print(AcY);
  Serial.print(" | AcZ= "); Serial.print(AcZ);
  Serial.print(" | GyX= "); Serial.print(GyX);
  Serial.print(" | GyY= "); Serial.print(GyY);
  Serial.print(" | GyZ= "); Serial.println(GyZ);
  delay(1);
}
