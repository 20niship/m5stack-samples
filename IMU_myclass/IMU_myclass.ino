#include <M5Stack.h>
#include "myIMU.h"
#define RAD_TO_DEG 57.324

float accX, accY, accZ;
float gyroX, gyroY, gyroZ;
float pitch, roll, yaw;
float my_pitch, my_roll, my_yaw;
float Temp;

myIMU MPU6885;

void setup(){
  M5.begin();
  M5.Power.begin();
    
  MPU6885.init();

  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextColor(GREEN , BLACK);
  M5.Lcd.setTextSize(2);
}

// the loop routine runs over and over again forever
void loop() {
    // put your main code here, to run repeatedly:
  MPU6885.getGyro(&gyroX,&gyroY,&gyroZ);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.printf("gyro=(%5.1f, %5.1f, %5.1f)", gyroX, gyroY, gyroZ);
  
  MPU6885.getAccel(&accX,&accY,&accZ);
  M5.Lcd.setCursor(0, 50);
  M5.Lcd.printf("acc=(%5.1f, %5.1f, %5.1f)", accX, accY, accZ);
  
  //reference = https://myenigma.hatenablog.com/entry/2015/11/09/183738
  float my_roll = atan(accY / accZ) * RAD_TO_DEG;
  float my_pitch = atan(-accX / sqrtf(accY*accY + accZ*accZ)) * RAD_TO_DEG;
  M5.Lcd.setCursor(0, 100);
  M5.Lcd.printf("pitch=%5.1f, roll=%5.1f)", my_pitch, my_roll);
  

  MPU6885.getTemperature(&Temp);
  M5.Lcd.setCursor(0, 200);
  M5.Lcd.printf("Temperature=%.2f C", Temp);

  
  delay(1);
}
