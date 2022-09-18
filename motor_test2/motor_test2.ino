
#include <M5Stack.h>
#include <Wire.h>


#define MOTOR_RPM           150
#define MAX_PWM             255
#define DEAD_ZONE           20

#define M5GO_WHEEL_ADDR     0x56  //https://www.switch-science.com/catalog/3995/ の写真参照
#define MOTOR_CTRL_ADDR     0x00  //
#define ENCODER_ADDR        0x04  //

class Motor{
public:
  Motor()          { _wire = &Wire; }
  Motor(TwoWire &w){ _wire = w; }
  ~Motor()         { stop(); }

  void setMotor(setMotor(int16_t pwm0, int16_t pwm1) {
      // Value range
      pwm_out0 = constrain(pwm0, -255, 255);
      pwm_out1 = constrain(pwm1, -255, 255);

      // Dead zone
      if (((pwm_out0 > 0) && (pwm_out0 < DEAD_ZONE)) || ((pwm_out0 < 0) && (pwm_out0 > -DEAD_ZONE))) pwm_out0 = 0;
      if (((pwm_out1 > 0) && (pwm_out1 < DEAD_ZONE)) || ((pwm_out1 < 0) && (pwm_out1 > -DEAD_ZONE))) pwm_out1 = 0;

      // Same value
      static int16_t pre_m0, pre_m1;
      if ((pwm_out0 == pre_m0) && (pwm_out1 == pre_m1))
        return;
      pre_m0 = pwm_out0;
      pre_m1 = pwm_out1;

      // Send I2C
      wire->beginTransmission(M5GO_WHEEL_ADDR);
      wire->write(MOTOR_CTRL_ADDR); // Motor ctrl reg addr
      wire->write(((uint8_t*)&pwm_out0)[0]);
      wire->write(((uint8_t*)&pwm_out0)[1]);
      wire->write(((uint8_t*)&pwm_out1)[0]);
      wire->write(((uint8_t*)&pwm_out1)[1]);
      wire->endTransmission();
  }
  inline void stop         ()                                                   { SetMotor(0,0); }
  inline void moveStraight (int16_t speed, uint16_t duration)                   { SetMotor(speed, speed); 	if (duration != 0) {delay(duration); stop(); } }
  inline void rotate       (uint16_t speed,  uint16_t duration, bool rotate_dir){ if(rotate_dir){SetMotor(speed, -speed);}else{SetMotor(-speed, speed);} 	if (duration != 0) {delay(duration); stop(); } };


  void readEncder() {
    static float _speed_input0 = 0, _speed_input1 = 0;
    int16_t rx_buf[2];

    //Get Data from Module.
    wire->beginTransmission(M5GO_WHEEL_ADDR);
    wire->write(ENCODER_ADDR); // encoder reg addr
    wire->endTransmission();
    wire->beginTransmission(M5GO_WHEEL_ADDR);
    wire->requestFrom(M5GO_WHEEL_ADDR, 4);

    if (wire->available()) {
      ((uint8_t*)rx_buf)[0] = wire->read();
      ((uint8_t*)rx_buf)[1] = wire->read();
      ((uint8_t*)rx_buf)[2] = wire->read();
      ((uint8_t*)rx_buf)[3] = wire->read();
      
      // フィルター（適当）
      _speed_input0 *= 0.9;
      _speed_input0 += 0.1 * rx_buf[0];
      _speed_input1 *= 0.9;
      _speed_input1 += 0.1 * rx_buf[1];
      
      speed_input0 = constrain((int16_t)(-_speed_input0), -255, 255);
      speed_input1 = constrain((int16_t)(_speed_input1), -255, 255);
    }
  }
  inline int16_t getSpeed0() { return speed_input0;}
  inline int16_t getSpeed1() { return speed_input1;}
  inline int16_t getMotor0() { return pwm_out0; }
  inline int16_t getMotor1() { return pwm_out1; }

private:
		TwoWire *_wire;
		int16_t speed_input0, speed_input1;
		int16_t pwm_out0, pwm_out1;
}


Motor motor(Wire);

void setup() {
  // Power ON Stabilizing...
  delay(500);
  M5.begin();

  // Init I2C
  Wire.begin();
  Wire.setClock(400000UL);  // Set I2C frequency to 400kHz
  delay(500);

  // Display info
  M5.Lcd.setTextFont(2);
  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.println("M5Stack Balance Mode start");

  // Init M5Bala
  motor.begin();
}

void loop() {
  M5.Lcd.setCursorPos(0,0);
  M5.Lcd.fillColor(BLACK);

  M5.Lcd.println("move forward -> 100%, 1sec !!");
  motor.moveStraight(255, 1000);

  M5.Lcd.println("stop moving");
  motor.stop();
  delay(1000);

  M5.Lcd.println("custom moving...");
  motor.setMotor(-100, 100);  delay(1000);
  motor.setMotor(80,   -55);  delay(1000);


  // M5 Loop
  M5.update();
}
