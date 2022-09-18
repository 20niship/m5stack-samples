
#include <M5Stack.h>

//M5StackFireとBala間のI2C通信を行うライブラリ
#include <Wire.h>

//再起動しても消えないようなデータを保存するためのライブラリ
#include <Preferences.h>

//M5BALAを動かすためのライブラリ
#include "imuCalibration.h"
#include "M5Bala.h"

//キャリブレーションデータをセーブするインタフェース
Preferences preferences;

//Wireで制御するm5balaを宣言
M5Bala m5bala(Wire);

//Dual Button用変数を宣言、初期値を設定
int old_value_red = 1;
int old_value_blue = 1;
int current_value_red = 1;
int current_value_blue = 1;

//姿勢情報をキャリブレーションする関数
void auto_tune_gyro_offset(){
  //開始の通知音を設定. 引数: 周波数, 時間
  M5.Speaker.tone(500, 200);
  
  delay(300);

  //内部でspeaker.update()が呼ばれる.
  M5.update();

  //キャリブレーションの開始をテキストで表示
  M5.Lcd.clear(BLACK);
  M5.Lcd.setTextSize(3);
  M5.Lcd.setCursor(0,0);
  M5.Lcd.println("Start Calibration");
  M5.Lcd.println("Don't Move");
  M5.Lcd.println("=================");
  
  delay(2000);
  
  //オフセットを計算する
  //X軸のみ。ただしimuCalibration.hと.cppに修正を加えればYZ軸も計算できる
  imu_calcGyroOffsets();
  
  //X軸の傾きオフセットを取得
  float gyroXoffset = imu_getOffsetX();
  
  //オフセットをセーブ
  preferences.putFloat("gyroXoffset", gyroXoffset);
  preferences.end();

  //終了の通知音を設定
  M5.Speaker.tone(1000, 200);
  delay(300);
  M5.update();

  //終了テキストを表示
  M5.Lcd.println("Finished");
  M5.Lcd.print("Xoffset = ");M5.Lcd.println(gyroXoffset);
  M5.Lcd.println("Please Restart");
}

//デフォルト画面を描画する関数.
void display_default_scene(){
  M5.Lcd.clear(BLACK);
  M5.Lcd.setTextSize(3);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setCursor(0,0);
  M5.Lcd.println("BALA-DualButton");
  M5.Lcd.println("===============");
  M5.Lcd.setTextSize(2);
  M5.Lcd.println("");
  M5.Lcd.println("Blue: Rotate ClockWise");
  M5.Lcd.println("");
  M5.Lcd.println("Red : Rotate AntiClockWise");
  M5.Lcd.setTextSize(3);
}

//起動時に一度だけ呼ばれる関数.
void setup() {
  //M5Stackの初期化
  delay(500);
  M5.begin();
  M5.Power.begin();

  //画面の描画 ==========================================================//
  display_default_scene();
  //===================================================================//
  
  //for Bala ==========================================================//
  //I2C通信の初期化
  Wire.begin();
  Wire.setClock(400000UL);
  delay(500);

  //M5Balaの初期化
  m5bala.begin();
  m5bala.setAngleOffset(-2.2);

  //キャリブレーション
  //IMUパラメータを新たにキャリブレーションする or セーブデータを使う
  if(M5.BtnC.isPressed()){
    //セーブデータを破棄（？）
    preferences.begin("m5bala-cfg", false);
    //キャリブレーション
    auto_tune_gyro_offset();
  }else{
    //セーブデータを使う
    preferences.begin("m5bala-cfg", true);
    //セットする
    imu_setOffsetX(preferences.getFloat("gyroXoffset"));
  }
  //===================================================================//

  //for Dual Button ===================================================//
  //26, 36番ピンを入力モードにする
  pinMode(26, INPUT);
  pinMode(36, INPUT);
  //===================================================================//
}

void loop() {
  //バランスを取りながら走る
  m5bala.run();

  //入力ピンから情報を読み取る-------------------------//
  // 押されている = 0, 押されていない = 1
  //赤ボタン
  current_value_red = digitalRead(26);
  //青ボタン
  current_value_blue = digitalRead(36);
  //-----------------------------------------------//

  //入力に従ってBalaの挙動を切り替える-----------------//
  //青ボタン
  if(current_value_blue != old_value_blue){
    // ボタンを押された状態が0
    if(current_value_blue == 0){
      //画面切替
      M5.Lcd.clear(BLUE);
      M5.Lcd.setCursor(0,0);
      M5.Lcd.println("Clockwise");
      M5.Lcd.println("");
      M5.Lcd.println("Rotation");
      //時計回りに回転. 引数はspeed, duration
      m5bala.rotate(70, 0);
    }else{
      //画面切替
      display_default_scene();
      //回転をストップ
      m5bala.stop();
    }
    //値の更新
    old_value_blue = current_value_blue;
  }
  //赤ボタン
  if(current_value_red != old_value_red){
    if(current_value_red == 0){
      //画面切替
      M5.Lcd.clear(RED);
      M5.Lcd.setCursor(0,0);
      M5.Lcd.println("AntiClockwise");
      M5.Lcd.println("");
      M5.Lcd.println("Rotation");
      //反時計回りに回転
      m5bala.rotate(-70,0);
    }else{
      //画面切替
      display_default_scene();
      //回転をストップ
      m5bala.stop();
    }
    //値の更新
    old_value_red = current_value_red;
  }
  //----------------------------------------------//
  //更新
  M5.update();
}
