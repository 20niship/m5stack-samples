// reference = https://github.com/m5stack/M5Stack/blob/master/examples/Unit/DUAL_BUTTON/DUAL_BUTTON.ino

#include <M5Stack.h>

#define BLUE_BUTTON_PIN 36
#define RED_BUTTON_PIN  26

int isBlueBtnPressed, isRedBtnPressed;
int last_BlueBtnPressed, last_RedBtnPressed;

void setup() {
  // init lcd
  M5.begin();
  M5.Power.begin();
  pinMode(BLUE_BUTTON_PIN, INPUT);
  pinMode(RED_BUTTON_PIN, INPUT);
  M5.Lcd.clear(BLACK);
  
  M5.Lcd.setBrightness(100); //バックライトの明るさを0（消灯）～255（点灯）で制御

  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(WHITE, BLACK);
}

void loop() {
  //digitalRead関数→ONの時は0より大きい数（実際には1？）、OFFの時は0以下を返す
  isBlueBtnPressed = digitalRead(BLUE_BUTTON_PIN);
  isRedBtnPressed  = digitalRead(RED_BUTTON_PIN);

  M5.Lcd.setCursor(0, 10);
  if(isBlueBtnPressed <= 0){
    M5.Lcd.println("Blue button -> ON ");
  }else{
    M5.Lcd.println("Blue button -> OFF ");
  }


  M5.Lcd.setCursor(0, 60);
  if(isRedBtnPressed <= 0){
    M5.Lcd.println("Red button -> ON ");
  }else{
    M5.Lcd.println("Red button -> OFF ");
  }

 M5.Lcd.setCursor(0, 110);
 if(isBlueBtnPressed != last_BlueBtnPressed){
    if(isBlueBtnPressed > 0){ M5.Lcd.println("Blue button pressed! ");}
    else {M5.Lcd.println("Blue button released! "); }

    last_BlueBtnPressed = isBlueBtnPressed;
  }else{
     M5.Lcd.println("                         ");
  }

    M5.Lcd.setCursor(0, 160);
  if(isRedBtnPressed != last_RedBtnPressed){
    if(isRedBtnPressed > 0){ M5.Lcd.println("Red button pressed! ");}
    else {M5.Lcd.println("Red button released! "); }

    last_RedBtnPressed = isRedBtnPressed;
  }else{
     M5.Lcd.println("                         ");
  }



  M5.update();
}
