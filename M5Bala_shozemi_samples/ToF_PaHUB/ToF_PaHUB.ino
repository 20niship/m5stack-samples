#include <Wire.h>
#include <M5Stack.h>
#include "ClosedCube_TCA9548A.h"

#define FRONT 2

#define X_LOCAL 100
#define Y_LOCAL 35
#define X_OFFSET 160
#define Y_OFFSET 34

#define PaHub_I2C_ADDRESS	0x70

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

int dist1;
int dist2;

void setup()
{
	M5.begin();
	M5.Power.begin();

    Wire.begin();
	M5.Lcd.fillScreen(TFT_BLACK); 
    tca9548a.address(PaHub_I2C_ADDRESS);
	M5.Lcd.setTextFont(4);
	M5.Lcd.setCursor(70, 0, 4);
	M5.Lcd.setTextColor(YELLOW,TFT_BLACK);
	M5.Lcd.println(("PaHUB Example"));
	M5.Lcd.setTextColor(TFT_WHITE,TFT_BLACK);
	
}


void loop()
{
	PaHUB();
}

void PaHUB(void){

  uint8_t returnCode = 0;
  uint8_t address;
  int count = 0;
  for( uint8_t channel=0; channel<TCA9548A_MAX_CHANNELS; channel++ ) {
    M5.Lcd.setCursor(X_LOCAL, Y_LOCAL + Y_OFFSET*channel , FRONT);
    M5.Lcd.printf("                                                                ");
    M5.Lcd.setCursor(X_LOCAL, Y_LOCAL + Y_OFFSET*channel , FRONT);
    M5.Lcd.printf("CH%d : ",channel);
    returnCode = tca9548a.selectChannel(channel);
    if( returnCode == 0 ) {
      for(address = 0x01; address < 0x7F; address++ ) {
        Wire.beginTransmission(address);
        returnCode = Wire.endTransmission();
        if (returnCode == 0) {
          //Serial.print("I2C device = ");
          if(address == 0x29){
            if(count == 0){
              dist1 = test();
              M5.Lcd.printf("%d",dist1);
              count = 1;
            }else{
              dist2 = test();
              M5.Lcd.printf("%d",dist2);
            }
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
