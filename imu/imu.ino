// define must ahead #include <M5Stack.h>
//#define M5STACK_MPU6886 
//#define M5STACK_MPU9250 
#define M5STACK_MPU6050
// #define M5STACK_200Q

#include <M5Stack.h>

#define RAD_TO_DEG 57.324

float accX, accY, accZ;
float gyroX, gyroY, gyroZ;
float pitch, roll, yaw;
float my_pitch, my_roll, my_yaw;
float Temp;


void setup(){
  M5.begin();
  M5.Power.begin();
    
  M5.IMU.Init();

  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextColor(GREEN , BLACK);
  M5.Lcd.setTextSize(2);
}

// the loop routine runs over and over again forever
void loop() {
    // put your main code here, to run repeatedly:
  M5.IMU.getGyroData(&gyroX,&gyroY,&gyroZ);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.printf("gyro=(%5.1f, %5.1f, %5.1f)", gyroX, gyroY, gyroZ);
  
  M5.IMU.getAccelData(&accX,&accY,&accZ);
  M5.Lcd.setCursor(0, 50);
  M5.Lcd.printf("acc=(%5.1f, %5.1f, %5.1f)", accX, accY, accZ);
  
  M5.IMU.getAhrsData(&pitch,&roll,&yaw);
  M5.Lcd.setCursor(0, 100);
  M5.Lcd.printf("PRY=(%5.1f, %5.1f, %5.1f)", pitch, roll, yaw);

  

  //reference = https://myenigma.hatenablog.com/entry/2015/11/09/183738
  float my_roll = atan(accY / accZ) * RAD_TO_DEG;
  float my_pitch = atan(-accX / sqrtf(accY*accY + accZ*accZ)) * RAD_TO_DEG;
  float my_yaw  = -atan(gyroY / gyroX);
  M5.Lcd.setCursor(0, 150);
  M5.Lcd.printf("PRY2=(%5.1f, %5.1f, %5.1f)", my_pitch, my_roll, gyroZ);


  M5.IMU.getTempData(&Temp);
  M5.Lcd.setCursor(0, 200);
  M5.Lcd.printf("Temperature=%.2f C", Temp);

  
  delay(1);
}
