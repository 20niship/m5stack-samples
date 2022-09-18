#include <M5Stack.h>

//setup()関数はM5Stackがスタートした最初に読み込まれる
void setup(){
  // M5Stack オブジェクトの初期化
  M5.begin();

  //Power chipがgpio21, gpio22, I2Cにつながれたデバイスに接続される。
  //バッテリー動作の場合はこの関数を読んでください（バッテリーの電圧を調べるらしい）
  M5.Power.begin();

  // 色々設定（なくても動作する）
  M5.Lcd.setCursor(10, 10); //文字表示の左上位置を設定
  M5.Lcd.setTextColor(RED); //文字色設定(WHITE, BLACK, RED, GREEN, BLUE, GRAY...)
  M5.Lcd.setTextSize(2);//文字の大きさ(px)設定
  
    
  // LCDディスプレイに文字表示
  M5.Lcd.drawString("こんにちは", 0, 0);
}

// loop()関数はsetup()関数が呼ばれた後に繰り返し呼ばれる。
// イメージとしては
/* void main(){
 *    setup();
 *    while(true){
 *       loop();
 *    }
 *  }
 */
 // みたいな感じ。

void loop() {
  //特になし！！
}
