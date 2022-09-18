#include <M5Stack.h>

#define NOTE_D0 -1
#define NOTE_D1 294
#define NOTE_D2 330
#define NOTE_D3 350
#define NOTE_D4 393
#define NOTE_D5 441
#define NOTE_D6 495
#define NOTE_D7 556

#define NOTE_DL1 147
#define NOTE_DL2 165
#define NOTE_DL3 175
#define NOTE_DL4 196
#define NOTE_DL5 221
#define NOTE_DL6 248
#define NOTE_DL7 278

#define NOTE_DH1 589
#define NOTE_DH2 661
#define NOTE_DH3 700
#define NOTE_DH4 786
#define NOTE_DH5 882
#define NOTE_DH6 990
#define NOTE_DH7 112


void setup() {
  M5.begin();
  M5.Power.begin();
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.fillScreen(BLACK);
}

void loop() {
  M5.Speaker.beep(); //beep音を鳴らす（この関数は音が鳴り始めるとすぐにメインループにreturunする）
  M5.Lcd.setCursor(0,0); 
  M5.Lcd.printf("beep()      ");// そのため、beep音が鳴り始めた直後に「beep」と表示される
  // 1病待つ（ビープ音を一定時間後に中止させるにはupdate()関数を呼ぶ必要がある）
  // delay(1000);にしてしまうと、update()が呼ばれるまで音が鳴り続ける
  for(int i=0;i<100;i++){
     M5.update();
     delay(10);
  }
  
  M5.Speaker.tone(NOTE_DH2, 200); //周波数= 3kHz, 長さ = 200ms
  M5.update();
  M5.Lcd.setCursor(0,0);
  M5.Lcd.printf("Custom beep ");
  //1病待つ（ビープ音を一定時間後に中止させるにはupdate()関数を呼ぶ必要がある）
  for(int i=0;i<100;i++){
     M5.update();
     delay(10);
  }
 
}
