
#include <M5Stack.h>

//M5StackFireとBala間のI2C通信を行うライブラリ
#include <Wire.h>

//PaHUBを使うためのライブラリ
#include "ClosedCube_TCA9548A.h"

//M5BALAを動かすためのライブラリ
#include "M5Bala.h"

//Wireで制御するm5balaを宣言
M5Bala m5bala(Wire);

//PaHUBを使うための定数の宣言
#define FRONT 2

#define PaHub_I2C_ADDRESS  0x70

#define VL53L0X_REG_IDENTIFICATION_MODEL_ID         0xc0
#define VL53L0X_REG_IDENTIFICATION_REVISION_ID      0xc2
#define VL53L0X_REG_PRE_RANGE_CONFIG_VCSEL_PERIOD   0x50
#define VL53L0X_REG_FINAL_RANGE_CONFIG_VCSEL_PERIOD 0x70
#define VL53L0X_REG_SYSRANGE_START                  0x00
#define VL53L0X_REG_RESULT_INTERRUPT_STATUS         0x13
#define VL53L0X_REG_RESULT_RANGE_STATUS             0x14
#define address1 0x29

ClosedCube::Wired::TCA9548A tca9548a;

byte gbuf[16];


int dist1; //小さい方のチャンネルに接続されたTOFの値
int dist2; //大きい方のチャンネルに接続されたTOFの値




void setup() {
  //M5Stackの初期化
  delay(500);
  M5.begin();
  M5.Power.begin();
  
  //for Bala ==========================================================//
  //I2C通信の初期化
  Wire.begin();
  Wire.setClock(400000UL);
  delay(500);

  //M5Balaの初期化
  m5bala.begin();
  m5bala.setAngleOffset(-2.2);
  //===================================================================//

  M5.Lcd.fillScreen(TFT_BLACK);
  tca9548a.address(PaHub_I2C_ADDRESS);
  
  dist1 = 0;
  dist2 = 0;
}

void loop() {
  //センサのデータ取得。dist1とdist2に値が入る
  PaHUB();

  //入力に従ってBalaの挙動を切り替える-----------------//
  if(dist1 < 200 || dist2 < 200){ //dist1とdist2のどちらかが200より小さければ
    M5.Lcd.clear(RED);
    M5.Lcd.setCursor(30,80);
    M5.Lcd.println("STOP");
    m5bala.setMotor(0,0); //モーターの出力を左右とも0に
  }else{ //そうでなければ(dist1とdist2の値が両方200以上なら)
    M5.Lcd.clear(BLACK);
    M5.Lcd.setCursor(30,80);
    M5.Lcd.println("GO");
    m5bala.setMotor(100,100); //モーターの出力を左右とも100に
  }
  //----------------------------------------------//

  //画面にセンサ値を表示しておく-----------------------//
  M5.Lcd.setCursor(10,100);
  M5.Lcd.setTextSize(3);
  M5.Lcd.setCursor(10,0);
  M5.Lcd.printf("dist1 = %d",dist1);
  M5.Lcd.setCursor(10,30);
  M5.Lcd.printf("dist2 = %d",dist2);
  //-----------------------------------------------//

  //更新
  M5.update();
}

//以下は書き換える必要なし



void PaHUB(void){

  uint8_t returnCode = 0;
  uint8_t address;
  int count = 0;
  for( uint8_t channel=0; channel<TCA9548A_MAX_CHANNELS; channel++ ) {
    returnCode = tca9548a.selectChannel(channel);
    if( returnCode == 0 ) {
      for(address = 0x01; address < 0x7F; address++ ) {
        Wire.beginTransmission(address);
        returnCode = Wire.endTransmission();
        if (returnCode == 0 && address == 0x29) {
            if(count == 0){
                dist1 = test();
                count = 1;
            }else{
                dist2 = test();
            }
        }
      }
    }
    delay(100);
  }
  count = 0;
}


int test() {
  byte val1 = read_byte_data_at(VL53L0X_REG_IDENTIFICATION_REVISION_ID);
  Serial.print("Revision ID: "); Serial.println(val1);

  val1 = read_byte_data_at(VL53L0X_REG_IDENTIFICATION_MODEL_ID);
  Serial.print("Device ID: "); Serial.println(val1);

  val1 = read_byte_data_at(VL53L0X_REG_PRE_RANGE_CONFIG_VCSEL_PERIOD);
  Serial.print("PRE_RANGE_CONFIG_VCSEL_PERIOD="); Serial.println(val1);
  Serial.print(" decode: "); Serial.println(VL53L0X_decode_vcsel_period(val1));

  val1 = read_byte_data_at(VL53L0X_REG_FINAL_RANGE_CONFIG_VCSEL_PERIOD);
  Serial.print("FINAL_RANGE_CONFIG_VCSEL_PERIOD="); Serial.println(val1);
  Serial.print(" decode: "); Serial.println(VL53L0X_decode_vcsel_period(val1));

  write_byte_data_at(VL53L0X_REG_SYSRANGE_START, 0x01);

  byte val = 0;
  int cnt = 0;
  while (cnt < 100) { // 1 second waiting time max
    delay(10);
    val = read_byte_data_at(VL53L0X_REG_RESULT_RANGE_STATUS);
    if (val & 0x01) break;
    cnt++;
  }
  if (val & 0x01) Serial.println("ready"); else Serial.println("not ready");

  read_block_data_at(0x14, 12);
  uint16_t acnt = makeuint16(gbuf[7], gbuf[6]);
  uint16_t scnt = makeuint16(gbuf[9], gbuf[8]);
  uint16_t dist = makeuint16(gbuf[11], gbuf[10]);
  byte DeviceRangeStatusInternal = ((gbuf[0] & 0x78) >> 3);
  return dist;

}

uint16_t bswap(byte b[]) {
  // Big Endian unsigned short to little endian unsigned short
  uint16_t val = ((b[0] << 8) & b[1]);
  return val;
}

uint16_t makeuint16(int lsb, int msb) {
    return ((msb & 0xFF) << 8) | (lsb & 0xFF);
}

void write_byte_data(byte data) {
  Wire.beginTransmission(address1);
  Wire.write(data);
  Wire.endTransmission();
}

void write_byte_data_at(byte reg, byte data) {
  // write data word at address1 and register
  Wire.beginTransmission(address1);
  Wire.write(reg);
  Wire.write(data);
  Wire.endTransmission();
}

void write_word_data_at(byte reg, uint16_t data) {
  // write data word at address1 and register
  byte b0 = (data &0xFF);
  byte b1 = ((data >> 8) && 0xFF);

  Wire.beginTransmission(address1);
  Wire.write(reg);
  Wire.write(b0);
  Wire.write(b1);
  Wire.endTransmission();
}

byte read_byte_data() {
  Wire.requestFrom(address1, 1);
  while (Wire.available() < 1) delay(1);
  byte b = Wire.read();
  return b;
}

byte read_byte_data_at(byte reg) {
  //write_byte_data((byte)0x00);
  write_byte_data(reg);
  Wire.requestFrom(address1, 1);
  while (Wire.available() < 1) delay(1);
  byte b = Wire.read();
  return b;
}

uint16_t read_word_data_at(byte reg) {
  write_byte_data(reg);
  Wire.requestFrom(address1, 2);
  while (Wire.available() < 2) delay(1);
  gbuf[0] = Wire.read();
  gbuf[1] = Wire.read();
  return bswap(gbuf);
}

void read_block_data_at(byte reg, int sz) {
  int i = 0;
  write_byte_data(reg);
  Wire.requestFrom(address1, sz);
  for (i=0; i<sz; i++) {
    while (Wire.available() < 1) delay(1);
    gbuf[i] = Wire.read();
  }
}


uint16_t VL53L0X_decode_vcsel_period(short vcsel_period_reg) {
  // Converts the encoded VCSEL period register value into the real
  // period in PLL clocks
  uint16_t vcsel_period_pclks = (vcsel_period_reg + 1) << 1;
  return vcsel_period_pclks;
}
