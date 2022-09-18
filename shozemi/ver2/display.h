#ifndef DISPLAY_H
#define DISPLAY_H

#include <M5Stack.h>
#include "common.h"

#define MAX_LEN 120
class myDisplay {
    float _pitch[MAX_LEN];
    float _roll[MAX_LEN];
    float _yaw[MAX_LEN];
    int _pry_start;
    
public:
    myDisplay();
    void init();
    void clear();
    void show_PRY();
    void setPRY(float pitch_, float roll_, float yaw_);
    void set_command_text(const char *prevCmd);
    void show_actions(uint8_t curAction, bool isCamON, bool isDelete_back);
    void show_status(uint8_t mode, bool isCamON, bool isDelete_back, bool isConnected);
    
//    void show_status();
//    void show_command(const char *command);
//    void show_battery(int level);
//    void show_mic_level(int level);
//    void show_out_level(int level);
//    void show_actions(Status status);
};

#endif
