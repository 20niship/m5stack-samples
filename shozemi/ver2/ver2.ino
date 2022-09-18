#define M5STACK_MPU6886 
#include <M5Stack.h>

#include "VL53L0X.h"
#include "display.h"
#include "communicate.h"
#include "common.h"


#define BLUE_BUTTON_PIN 36
#define RED_BUTTON_PIN  26

int isBlueBtnPressed, isRedBtnPressed;
int last_BlueBtnPressed, last_RedBtnPressed;

uint8_t cur_mode;
bool isCamON;
bool isDelete_back;
bool isConnected;
bool isDoingActoin;
uint8_t cur_selected_action;


/* ###############################################################
##########              メインのプログラム     　　  ###############
############################################################### */
VL53L0X sensor;
myUDPcom comm{};
myDisplay disp{};


void do_action(uint8_t command){
  int dist;
  switch(command){
    case Cmd_next_slide:
    disp.set_command_text("next");
    comm.send_OneData(SndData_Cmd, Cmd_next_slide);
    break;

    case Cmd_prev_slide:
    disp.set_command_text("prev");
    comm.send_OneData(SndData_Cmd, Cmd_prev_slide);
    break;

    case Cmd_zoom_img:
    disp.set_command_text("zoom");
    comm.send_OneData(SndData_Cmd, Cmd_zoom_img);
    break;

    case Cmd_load_stl:
    disp.set_command_text("STL");
    comm.send_OneData(SndData_Cmd, Cmd_load_stl);
    break;

    case Cmd_show_slide_archive:
    disp.set_command_text("archive");
    comm.send_OneData(SndData_Cmd, Cmd_show_slide_archive);
    break;

//    case Cmd_set_volume:
//    dist = sensor.readRangeSingleMillimeters();
//    if((dist < 1000) && (dist > 30)){
//      disp.set_command_text("vol");
//      comm.send_OneData(SndData_vol_out, constrain(dist, 0, 200));
//    }
//    break;

    case Cmd_cam_on:
    comm.send_OneData(SndData_Cmd, Cmd_cam_on);
    disp.set_command_text("cam on");
    isCamON = true;
    disp.show_actions(cur_selected_action, isCamON, isDelete_back);
    break;

    case Cmd_cam_off:
    comm.send_OneData(SndData_Cmd, Cmd_cam_off);
    disp.set_command_text("cam off");
    isCamON = false;
    disp.show_actions(cur_selected_action, isCamON, isDelete_back);
    break;

    case Cmd_del_bk:
     comm.send_OneData(SndData_Cmd, Cmd_del_bk);
     isDelete_back = true;
    disp.show_actions(cur_selected_action, isCamON, Cmd_not_del_bk);
    break;

    case Cmd_not_del_bk:
     comm.send_OneData(SndData_Cmd, Cmd_not_del_bk);
     isDelete_back = false;
    disp.show_actions(cur_selected_action, isCamON, Cmd_not_del_bk);
    break;
  }
}


uint8_t getActionEnum_from_ActionBtn(uint8_t action, bool isCamON, bool isDelete_back){
   switch(action){
        case 0: 
            if(isCamON) return Cmd_cam_off;
            else return Cmd_cam_on;
            break;

        case 1:
            if(isDelete_back) return Cmd_not_del_bk;
            else return Cmd_del_bk;
            break;

        case 2: return Cmd_load_stl;
        case 3: return Cmd_set_volume;
        case 4: return Cmd_set_mic_level;
        case 5: return Cmd_Exit;

        default: return 0xFF;
    }
}


void setup() {
  cur_mode = 0;
  isCamON = false;
  isConnected = false;
  isDelete_back = false;
  isDoingActoin = false;
  cur_selected_action = 0;
  
  M5.begin();
  M5.Power.begin();

  Serial.begin(115200);
  
  pinMode(BLUE_BUTTON_PIN, INPUT);
  pinMode(RED_BUTTON_PIN, INPUT);
  M5.IMU.Init();
  
  //Power chipがgpio21, gpio22, I2Cにつながれたデバイスに接続される。
  //バッテリー動作の場合はこの関数を読んでください（バッテリーの電圧を調べるらしい）
  Wire.begin();// I2C通信を開始する
  sensor.init();
  sensor.setTimeout(10);
  comm.setup();
  disp.init();
  disp.clear();
  disp.set_command_text("setup");
  disp.show_actions(cur_selected_action, isCamON, isDelete_back);
}


inline int getMotion(float gyroX, float gyroZ){
  //gyroZ > 400 -> Next;
  static unsigned long last_time = millis();
  static unsigned long now_time;
  static bool isStandby = true;

  now_time = millis();
  if((gyroZ < 100) &&(gyroX < 100)) isStandby = true;

  if((now_time - last_time < 100) || (isStandby == false)) return -1;

  last_time = now_time;
  int motion = -1;
    
  if     (gyroZ >  400){ isStandby = false; motion = Cmd_next_slide; }
  else if(gyroZ < -400){ isStandby = false; motion = Cmd_prev_slide; }
  else if(gyroX >  400){ isStandby = false; motion = Cmd_show_slide_archive; }
  else if(gyroX < -400){ isStandby = false; motion = Cmd_zoom_img; }

  Serial.println(motion);
  
  return motion;
}


void button_actions(){
  isBlueBtnPressed = digitalRead(BLUE_BUTTON_PIN);
  isRedBtnPressed  = digitalRead(RED_BUTTON_PIN);
  /* ###############################################################
  ##########        青ボタン ＝ 押している時はDrawing  ##############
  ############################################################### */
  if(isBlueBtnPressed <= 0){
  }

 if(isBlueBtnPressed != last_BlueBtnPressed){
    if(isBlueBtnPressed > 0){ //released
     disp.set_command_text("stp dr");
    comm.send_OneData(SndData_Cmd, Cmd_Stop_Draw);
    }else{
      disp.set_command_text("draw!");
      comm.send_OneData(SndData_Cmd, Cmd_Start_Draw);
    }
    last_BlueBtnPressed = isBlueBtnPressed;
  }



  /* ###############################################################
  ##########      赤ボタン ＝ 押している時だけマイクON  ##############
  ############################################################### */
  if(isRedBtnPressed <= 0){
     int distance = sensor.readRangeSingleMillimeters();
     disp.set_command_text("mic on");
     comm.send_OneData(SndData_vol_out, constrain(distance/3, 0, 100)); 
     delay(100);
  }


//  if(isRedBtnPressed != last_RedBtnPressed){
//    if(isRedBtnPressed > 0){//released
//     disp.set_command_text("mic off");
//     comm.send_OneData(SndData_vol_mic, 0); 
//    }
//    last_RedBtnPressed = isRedBtnPressed;
//  }


  /* ###############################################################
  ##########   　　　　　   BtnA -> mode変更　　　　　  ##############
  ############################################################### */
  if (M5.BtnA.wasPressed()) {
    cur_mode++; 
    if(cur_mode >= NUM_MODE) cur_mode = 0;
    disp.show_status(cur_mode, isCamON, isDelete_back, isConnected);
    switch(cur_mode){
      case Mode_PP:       comm.send_OneData(SndData_Cmd, Cmd_mode_pp);break;
      case Mode_Monitor:  comm.send_OneData(SndData_Cmd, Cmd_mode_monitor);break;
      case Mode_PlainImg: comm.send_OneData(SndData_Cmd, Cmd_mode_blank); break;   
    }
  }
  

 /* ###############################################################
  ##########   　　　　　   BtnB -> Action実行　　　　  ##############
  ############################################################### */
  if (M5.BtnB.wasPressed()) {
    uint8_t command_ = getActionEnum_from_ActionBtn(cur_selected_action, isCamON, isDelete_back);
    do_action(command_);
  }
  
  if (M5.BtnC.wasPressed()) {
    cur_selected_action++;
    if(cur_selected_action >= NUM_ACTION) cur_selected_action = 0;

    disp.show_actions(cur_selected_action, isCamON, isDelete_back);
  }
}


void loop(){
  M5.update();// update button state

  /* ###############################################################
  ##########              メインのプログラム     　　  ##############
  ############################################################### */

  // PRY関連、モーション検出、位置推定
  float pitch, roll, yaw;
  float accX, accY, accZ;
  float gyroX, gyroY, gyroZ;

  M5.IMU.getAhrsData(&pitch,&roll,&yaw);
  pitch = constrain(pitch, -90.0, 90.0);
  roll  = constrain(roll, -90.0, 90.0);
  yaw   = constrain(yaw, -90.0, 90.0);
  
  //M5.IMU.getAccelData(&accX,&accY,&accZ);
  M5.IMU.getGyroData(&gyroX, &gyroY, &gyroZ);
  disp.setPRY(pitch, roll, yaw);
  disp.show_PRY();

  comm.send_three_data(SndData_PRY, uint8_t(pitch+90), uint8_t(roll+90), uint8_t(yaw+90));
  button_actions();
  
  int motion = getMotion(gyroX, gyroZ);
  do_action(motion);

  delay(100);
}
