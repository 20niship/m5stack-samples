// reference = https://github.com/m5stack/M5Stack/blob/master/examples/Unit/LIGHT/LIGHT.ino
// reference2= https://docs.m5stack.com/#/en/unit/light

#include <M5Stack.h>

void setup() {
  M5.begin();
  M5.Power.begin();

  dacWrite(25, 0);  //disable the speak noise

  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.setTextSize(2);
  M5.Lcd.fillScreen(BLACK);

  pinMode(26, INPUT); //GPIOピンの26をデジタル入力に設定
  // アナログ入力はpinMode関数を呼ぶは必要ない
}

uint16_t analogRead_value = 0;
uint16_t digitalRead_value = 0;
void loop() {
  analogRead_value = analogRead(36); //0～1023までの整数でピンの電圧測定
  digitalRead_value = digitalRead(26); //デジタル入力
  
  M5.Lcd.setCursor(10, 10);
  M5.Lcd.printf("analog = %d   ", analogRead_value);
  M5.Lcd.setCursor(10, 70);
  M5.Lcd.printf("digital = %d  ", digitalRead_value);
  delay(10);
}
