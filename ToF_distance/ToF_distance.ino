#include <M5Stack.h>
#include <Wire.h>
#include "VL53L0X.h"

//reference = https://qiita.com/nnn112358/items/1d71c0b3269ee977e781
//reference = https://twitter.com/tokuhira/status/1094103814347472923?s=20
//library   = https://github.com/pololu/vl53l0x-arduino

// →を使ってもできるかも（https://github.com/m5stack/M5Stack/blob/master/examples/Unit/ToF_VL53L0X/ToF_VL53L0X.ino）

VL53L0X sensor;
void setup() {
  M5.begin();

  //Power chipがgpio21, gpio22, I2Cにつながれたデバイスに接続される。
  //バッテリー動作の場合はこの関数を読んでください（バッテリーの電圧を調べるらしい）
  M5.Power.begin();

  Wire.begin();
  
  sensor.init();
  sensor.setTimeout(500);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(10, 10);
  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.setTextSize(4);
}
void loop() {
  int distance = sensor.readRangeSingleMillimeters();
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.print(distance);
  M5.Lcd.print("[mm]");
    
  if (!sensor.timeoutOccurred()) {
    M5.Lcd.setCursor(0, 50);
    M5.Lcd.println("TIMEOUT");
  }else{
    M5.Lcd.setCursor(0, 50);
    M5.Lcd.println("        ");  
  }
}
