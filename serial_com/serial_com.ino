#include <M5Stack.h>

void setup() {
  M5.begin(true, false, true);
  M5.Power.begin();

  Serial.begin(115200);//ビットレート設定 

  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.setTextSize(2);
}

void repeat_string(){
  if ( Serial.available() > 0 ) {
    String str = Serial.readStringUntil('\n');
    Serial.print("recieved -> ");
    Serial.println(str);
  }
}

void SendTest(){
  Serial.print("BIN->"); 
  Serial.print(10, BIN); // 2進数の10である"1010"が文字列として送信される
  Serial.print("\nOCT->"); 
  Serial.print(10, OCT); // 8進数の10である"12"が文字列として送信される
  Serial.print("\nDEC->"); 
  Serial.print(10, DEC); // 10進数の10である"10"が文字列として送信される
  Serial.print("\nHEX->"); 
  Serial.print(10, HEX); // 16進数の10である"A"が文字列として送信される
  Serial.print("\n"); 
  delay(1000);
}


void repeat_byte(){
  uint8_t data[] = {10, 20, 30, 40};
  Serial.write(data, 4); 
}

void display_recieved_data(){
  #define MAX_SIZE 100
  int recieved_byte_size = Serial.available();
  char recieved_data[100];
  
  for(int i=0;i<recieved_byte_size;i++){
    recieved_data[i] = (char)Serial.read();
  }
  recieved_data[recieved_byte_size] = 0; //終端文字
  
  //描画！
  M5.Lcd.setCursor(0,100);
  M5.Lcd.printf("%s", recieved_data);
}

int serial_status = 0;

void loop() {
  M5.update();// update button state

  M5.Lcd.setCursor(0,0);
  if (M5.BtnA.wasPressed()) {
    serial_status = 0;
    M5.Lcd.print("button A \n-> repeat_string() ");
  } 
  if (M5.BtnB.wasPressed()) {
    serial_status = 1;
    M5.Lcd.print("button B \n-> SendTest()      ");
  }

  if (M5.BtnC.wasPressed()) {
    serial_status = 2;
    M5.Lcd.print("button C \n-> repeat_byte()    ");
  }

  switch(serial_status){
  case 0:
    repeat_string();
    break;
  case 1:
    SendTest();
    break;
  case 2:
    repeat_byte();
    //display_recieved_data();
    break;
  }
}
