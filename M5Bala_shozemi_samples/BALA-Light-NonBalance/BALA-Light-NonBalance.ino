
#include <M5Stack.h>

//M5StackFireとBala間のI2C通信を行うライブラリ
#include <Wire.h>

//M5BALAを動かすためのライブラリ
#include "M5Bala.h"

//Wireで制御するm5balaを宣言
M5Bala m5bala(Wire);

//Light Unitのセンサ値を格納する変数
uint16_t analogRead_value = 0;
uint16_t digitalRead_value = 0;

//起動時に一度だけ呼ばれる関数.
void setup() {
  //M5Stackの初期化
  delay(500);
  M5.begin();
  M5.Power.begin();

  //画面の描画 ==========================================================//
  M5.Lcd.clear(BLACK);
  M5.Lcd.setTextSize(3);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setCursor(0,0);
  M5.Lcd.println("BALA-Light");
  M5.Lcd.println("===============");
  //===================================================================//
  
  //for Bala ==========================================================//
  //I2C通信の初期化
  Wire.begin();
  Wire.setClock(400000UL);
  delay(500);

  //M5Balaの初期化
  m5bala.begin();
  m5bala.setAngleOffset(-2.2);
  //===================================================================//

  //for Light Unit ===================================================//
  //スピーカーノイズを消す
  dacWrite(25,0);
  //26, 36番ピンを入力モードにする
  pinMode(26, INPUT);
  pinMode(36, INPUT);
  //===================================================================//
}

void loop() {
  delay(10);
  
  //入力ピンから情報を読み取る-------------------------//
  analogRead_value = analogRead(36);
  digitalRead_value = digitalRead(26);
  //-----------------------------------------------//

  //画面にセンサ値を表示しておく-----------------------//
  M5.Lcd.fillRect(50,50,200,100,BLACK);
  M5.Lcd.setCursor(0,50);
  M5.Lcd.setTextSize(3);
  M5.Lcd.printf("A: ");M5.Lcd.println(analogRead_value);
  M5.Lcd.printf("D: ");M5.Lcd.println(digitalRead_value);
  //-----------------------------------------------//
  
  //入力に従ってBalaの挙動を切り替える-----------------//
  if (analogRead_value < 500) {
    m5bala.setMotor(100,-100);
    delay(500);
    m5bala.setMotor(200,200);
    delay(1000);
  }else{
    m5bala.setMotor(0,0);
  }
  //----------------------------------------------//
  //更新
  M5.update();
}
