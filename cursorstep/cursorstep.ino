// MPU-6050 Short Example Sketch
// By Arduino User JohnChi - August 17, 2014
// Edit by Nitcha Tothong

// OF alt technique
//https://www.youtube.com/watch?v=6BKTocYpkCw
//https://github.com/hagino3000/KinectJesture

#include <Wire.h>
#include <Mouse.h>

int counter = 0;
const int MPU = 0x68; // I2C address of the MPU-6050
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;

boolean bDidType = false;

void setup() {

  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  Serial.begin(9600);
  Mouse.begin();
}

void loop() {

  //-----Notification Input from Sensor
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 14, true); // request a total of 14 registers
  AcX = Wire.read() << 8 | Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  AcY = Wire.read() << 8 | Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ = Wire.read() << 8 | Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp = Wire.read() << 8 | Wire.read(); // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX = Wire.read() << 8 | Wire.read(); // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY = Wire.read() << 8 | Wire.read(); // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ = Wire.read() << 8 | Wire.read(); // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)

  Serial.print(" | AcX = "); Serial.print(AcX);
  Serial.print(" | AcY = "); Serial.print(AcY);
  Serial.print(" | AcZ = "); Serial.print(AcZ);
  Serial.print(" | Tmp = "); Serial.print(Tmp / 340.00 + 36.53); //equation for temperature in degrees C from datasheet
  Serial.print(" | GyX = "); Serial.print(GyX);
  Serial.print(" | GyY = "); Serial.print(GyY);
  Serial.print(" | GyZ = "); Serial.println(GyZ);


  //-----Actions - Click

  if (AcX > 10000) {
    if (!bDidType) {
      counter++;
      Mouse.click();
      Mouse.end();
      bDidType = true;
    }
  }
  else {
    bDidType = false;
    //      Keyboard.println("Good");
  }

  //-----Actions - Mouse Move
  
  // Right
  if (AcY < 15000 && AcY > 1500 ) {
    Mouse.move(20, 1, 3);
  }
  // Left
  else if (AcY > -15000 && AcY < -1500 ) {
    Mouse.move(-20, 0, 0);
  }
 
  // Up
  if (AcX > -7000 && AcX < 2000 ) {
    Mouse.move(0, -15, 0);
  }
  // Down
  else if (AcX < 7000 && AcX > -2000 ) {
    Mouse.move(0, 15, 0);
  }




  delay(103);
}

