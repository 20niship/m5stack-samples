//reference = https://github.com/m5stack/M5Bala
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
  Motor()         { _wire = &Wire; };
  Motor(TwoWire &w){ _wire = &w; };
  ~Motor()        { stop(); };

  void setMotor(int16_t pwm0, int16_t pwm1) {
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
      
      M5.Lcd.printf("write(%d, %d, %d, %d)", ((uint8_t*)&pwm_out0)[0], 
                                ((uint8_t*)&pwm_out0)[1],
                                ((uint8_t*)&pwm_out1)[0],
                                ((uint8_t*)&pwm_out1)[1]);
      M5.Lcd.println("");
      
      // Send I2C
      _wire->beginTransmission(M5GO_WHEEL_ADDR);
      _wire->write(MOTOR_CTRL_ADDR); // Motor ctrl reg addr
      _wire->write(((uint8_t*)&pwm_out0)[0]);
      _wire->write(((uint8_t*)&pwm_out0)[1]);
      _wire->write(((uint8_t*)&pwm_out1)[0]);
      _wire->write(((uint8_t*)&pwm_out1)[1]);
      _wire->endTransmission();
  };
  inline void stop         ()                                                   { setMotor(0,0); };
  inline void moveStraight (int16_t speed, uint16_t duration)                   { setMotor(speed, speed); 	if (duration != 0) {delay(duration); stop(); } };
  inline void rotate       (uint16_t speed,  uint16_t duration, bool rotate_dir){ if(rotate_dir){setMotor(speed, -speed);}else{setMotor(-speed, speed);} 	if (duration != 0) {delay(duration); stop(); } };


  void readEncder() {
    static float _speed_input0 = 0, _speed_input1 = 0;
    int16_t rx_buf[2];

    //Get Data from Module.
    _wire->beginTransmission(M5GO_WHEEL_ADDR);
    _wire->write(ENCODER_ADDR); // encoder reg addr
    _wire->endTransmission();
    _wire->beginTransmission(M5GO_WHEEL_ADDR);
    _wire->requestFrom(M5GO_WHEEL_ADDR, 4);

    if (_wire->available()) {
      ((uint8_t*)rx_buf)[0] = _wire->read();
      ((uint8_t*)rx_buf)[1] = _wire->read();
      ((uint8_t*)rx_buf)[2] = _wire->read();
      ((uint8_t*)rx_buf)[3] = _wire->read();
      
      // filter
      _speed_input0 *= 0.9;
      _speed_input0 += 0.1 * rx_buf[0];
      _speed_input1 *= 0.9;
      _speed_input1 += 0.1 * rx_buf[1];
      
      speed_input0 = constrain((int16_t)(-_speed_input0), -255, 255);
      speed_input1 = constrain((int16_t)(_speed_input1), -255, 255);
    }
  };
  inline int16_t getSpeed0() { return speed_input0;};
  inline int16_t getSpeed1() { return speed_input1;};
  inline int16_t getMotor0() { return pwm_out0; };
  inline int16_t getMotor1() { return pwm_out1; };

private:
		TwoWire *_wire;
		int16_t speed_input0, speed_input1;
		int16_t pwm_out0, pwm_out1;
};


Motor motor(Wire);
//Motor motor;

void setup() {
  // Power ON Stabilizing...
  delay(500);
  M5.begin();
  M5.Power.begin();

  // Init I2C
  Wire.begin();
  Wire.setClock(400000UL);  // Set I2C frequency to 400kHz
  delay(500);

  // Display info
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.setCursor(0,0);
  M5.Lcd.println("M5Stack Balance Mode start");
}



void loop(){
  M5.Lcd.setCursor(0,0);
  M5.Lcd.fillScreen(BLACK);

  M5.Lcd.println("move forward -> 100%, 1sec !!");
  motor.setMotor(255, 255);
  delay(3000);

  M5.Lcd.println("stop moving");
  motor.stop();
  delay(3000);

  M5.Lcd.println("custom moving...");
  motor.setMotor(-255, -255);  
  delay(3000);

  
  M5.Lcd.println("custom moving...");
  motor.setMotor(80,-55);  
  delay(3000);


  // M5 Loop
  M5.update();

}
