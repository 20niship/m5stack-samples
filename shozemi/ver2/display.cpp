#include "display.h"


myDisplay::myDisplay(){}

void myDisplay::init(){
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(0,0);
  M5.Lcd.setTextColor(WHITE, BLACK); //文字色を設定（文字背景色は透明）
  M5.Lcd.setBrightness(100); //バックライトの明るさを0（消灯）～255（点灯）で制御
  
  M5.Lcd.fillScreen(BLACK);

  for(int i=0; i<MAX_LEN; i++){
      _pitch[i] = 0;
      _roll[i] = 0;
      _yaw[i] = 0;
  }
  _pry_start = 0;
}

void myDisplay::clear(){
  uint16_t col_blue_line = M5.Lcd.color565(131, 240, 115); //RGB値（0～255）を指定

  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0,  0); M5.Lcd.print("mode");
  M5.Lcd.setCursor(0, 20); M5.Lcd.print("cam");
  M5.Lcd.setCursor(0, 40); M5.Lcd.print("del bk");
  M5.Lcd.setCursor(0, 60); M5.Lcd.print("status");

  M5.Lcd.setCursor(3, 105); M5.Lcd.print("CMD");
  M5.Lcd.setCursor(3, 125); M5.Lcd.print("PWR");
  M5.Lcd.setCursor(3, 149); M5.Lcd.print("MIC");
  M5.Lcd.setCursor(3, 178); M5.Lcd.print("OUT");

  M5.Lcd.drawRect(50, 105, 98, 18, col_blue_line); 
  M5.Lcd.drawRect(50, 133, 98, 18, col_blue_line); 
  M5.Lcd.drawRect(50, 156, 98, 18, col_blue_line);
  M5.Lcd.drawRect(50, 181, 98, 18, col_blue_line);
  
  M5.Lcd.drawFastHLine(0, 200, 320, col_blue_line);
  M5.Lcd.drawFastHLine(0, 97, 320, col_blue_line);
  M5.Lcd.drawFastVLine(90, 201, 50, col_blue_line);
  M5.Lcd.drawFastVLine(200, 201, 50, col_blue_line);
  M5.Lcd.drawFastVLine(73, 0,  95, col_blue_line);

  M5.Lcd.setCursor(0,   213); M5.Lcd.print("<Mode>");
  M5.Lcd.setCursor(110, 213); M5.Lcd.print("Action");
  M5.Lcd.setCursor(200, 213); M5.Lcd.print("<Action>");

}
  
void myDisplay::show_PRY(){  //(187,12)から、(120,60)の大きさ
  uint16_t now_pt;
  M5.Lcd.fillRect(186, 3, 125, 65, BLACK); // 塗りつぶし left, top, witdh, height

  for (int i = 1; i < MAX_LEN-1; i++) {
    now_pt = (_pry_start + i) % (MAX_LEN);

    M5.Lcd.drawLine(i + 187, _pitch[now_pt] + 3, i + 189, _pitch[(now_pt + 1) % MAX_LEN] + 3, TFT_YELLOW);
    M5.Lcd.drawLine(i + 187, _roll[now_pt]  + 3, i + 189,  _roll[(now_pt + 1) % MAX_LEN] + 3, TFT_BLUE );
    M5.Lcd.drawLine(i + 187, _yaw[now_pt]   + 3, i + 189,   _yaw[(now_pt + 1) % MAX_LEN] + 3, TFT_RED   );        
  }
}

void myDisplay::setPRY(float pitch_, float roll_, float yaw_){
  _pry_start++;
  if(_pry_start > MAX_LEN - 1) {_pry_start = 0; }
//  _pitch[_pry_start] = constrain(pitch_, -90.0, 90.0);
//  _roll[_pry_start]  = constrain(roll_, -90.0, 90.0);
//  _yaw[_pry_start]   = constrain(yaw_, -90.0, 90.0);

  _pitch[_pry_start] = map(pitch_, -90.0, 90.0, 0, 65);
  _roll[_pry_start]  = map(roll_,  -90.0, 90.0, 0, 65);
  _yaw[_pry_start]   = map(yaw_,   -90.0, 90.0, 0, 65);
}


//curAction --> 現在実行しようとしているコマンド
// 0 ～ NUM_ACTION-1までの値をとること
void myDisplay::show_actions(uint8_t curAction, bool isCamON, bool isDelete_back){
  M5.Lcd.setCursor(178, 86 ); 
  if(isCamON) M5.Lcd.print("CAM ON");
  else M5.Lcd.print("CAM OFF");

  M5.Lcd.setCursor(178, 106); 
  if(isDelete_back) M5.Lcd.print("del back");
  else M5.Lcd.print("show back");

  M5.Lcd.setCursor(178, 126); M5.Lcd.print("Load STL");
  M5.Lcd.setCursor(178, 146); M5.Lcd.print("Vol Out");
  M5.Lcd.setCursor(178, 166); M5.Lcd.print("Vol Mic");

  M5.Lcd.fillRect(155, 81, 16, 96, TFT_BLACK); 
  if(curAction >= NUM_ACTION) return;

  M5.Lcd.fillCircle(160, 86 + 20*curAction, 5, RED); //塗りつぶし center-x, center-y, radius 
}

void myDisplay::show_status(uint8_t mode, bool isCamON, bool isDelete_back, bool isConnected){
  M5.Lcd.fillRect(75, 0, 90, 90, TFT_BLACK); 

  M5.Lcd.setCursor(75, 0);
  switch(mode){
    case Mode_PP:       M5.Lcd.print("pp");     break;
    case Mode_Monitor:  M5.Lcd.print("monitor");break;
    case Mode_PlainImg: M5.Lcd.print("none");   break;
  }

  M5.Lcd.setCursor(75, 20);
  if(isCamON) M5.Lcd.print("ON");
  else M5.Lcd.print("OFF"); 

  M5.Lcd.setCursor(75, 40);
  if(isDelete_back) M5.Lcd.print("YES");
  else M5.Lcd.print("NO");

  M5.Lcd.setCursor(75, 60);
  if(isConnected) M5.Lcd.print("ONLINE");
  else M5.Lcd.print("OFFLINE");
}

void myDisplay::set_command_text(const char *prevCmd){
  M5.Lcd.fillRect(51, 106, 96, 16, TFT_BLACK); 
  M5.Lcd.setCursor(50, 105);
  M5.Lcd.printf("%s", prevCmd);
  //M5.Lcd.print("AAAAA");
}
