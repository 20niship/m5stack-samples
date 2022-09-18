# include "communicate.h"


myUDPcom::myUDPcom(){
    // arp -aでIPアドレスをすべて取得
    // ipconfigで自身のIPアドレスを取得
    wifi_ssid = "GlocalNet_0HG8TN"; //WiFIのSSIDを入力
    wifi_pass = "68866409"; // WiFiのパスワードを入力

    WiFiUDP wifiUdp; 
    pc_addr = "192.168.43.170";  //"192.168.0.6";
    pc_port = 50007; //送信先のポート
    my_port = 50008;  //自身のポート
}

void myUDPcom::setup(){
    WiFi.begin(wifi_ssid, wifi_pass);
    while( WiFi.status() != WL_CONNECTED) {
        delay(500); 
        M5.Lcd.print("."); 
    }  
    M5.Lcd.println("WiFi connected");
    M5.Lcd.print("IP address = ");
    M5.Lcd.println(WiFi.localIP());

    wifiUdp.begin(my_port);
}

#define START_COM_BYTE 0xFF
#define END_COM_BYTE   0xFE

void myUDPcom::send_three_data(uint8_t type, uint8_t data1, uint8_t data2, uint8_t data3){
  wifiUdp.beginPacket(pc_addr, pc_port);
  wifiUdp.write(START_COM_BYTE);
  uint8_t data_type = (3 << 5) | type;
  wifiUdp.write(data_type);
  wifiUdp.write(data1);
  wifiUdp.write(data2);
  wifiUdp.write(data3);
  wifiUdp.write(END_COM_BYTE);
  wifiUdp.endPacket();
}


void myUDPcom::send_OneData(uint8_t type, uint8_t data){
  wifiUdp.beginPacket(pc_addr, pc_port);
  wifiUdp.write(START_COM_BYTE);
  uint8_t data_type = (1 << 5) | type;
  wifiUdp.write(data_type);
  wifiUdp.write(data);
  wifiUdp.write(END_COM_BYTE);
  wifiUdp.endPacket();
}
