#include <WiFi.h>
#include <WiFiUDP.h>
#include <M5Stack.h>

#ifndef COMMUNICATION_H
#define COMMUNICATION_H

class myUDPcom {
private:
  // arp -aでIPアドレスをすべて取得
  // ipconfigで自身のIPアドレスを取得
  char *wifi_ssid; //WiFIのSSIDを入力
  char *wifi_pass; // WiFiのパスワードを入力

  WiFiUDP wifiUdp; 
  char *pc_addr;  //"192.168.0.6";
  int pc_port; //送信先のポート
  int my_port;  //自身のポート


public:
  myUDPcom();
  void setup();
  void send_three_data(uint8_t type, uint8_t gyroX, uint8_t gyroY, uint8_t gyroZ);
  void send_OneData(uint8_t type, uint8_t data);

};


#endif
