#include <WiFi.h>
#include <WiFiUDP.h>
#include <M5Stack.h>

// arp -aでIPアドレスをすべて取得
// ipconfigで自身のIPアドレスを取得

const char ssid[] = "GlocalNet_0HG8TN"; //"aterm-a5535b-g"; // SSID
const char pass[] = "68866409"; //"3f4842c397e8f";  // password

WiFiUDP wifiUdp; 
const char *pc_addr = "192.168.43.170";  //"192.168.0.6";
const int pc_port = 50007; //送信先のポート
const int my_port = 50008;  //自身のポート


void setup() {
  M5.begin();
  M5.Power.begin();
  M5.Lcd.setTextSize(2);

  WiFi.begin(ssid, pass);
  while( WiFi.status() != WL_CONNECTED) {
    delay(500); 
    M5.Lcd.print("."); 
  }  
  M5.Lcd.println("WiFi connected");
  M5.Lcd.print("IP address = ");
  M5.Lcd.println(WiFi.localIP());
  
  wifiUdp.begin(my_port);
}

void loop(){
  int num_packet = wifiUdp.parsePacket();
  char packetBuffer[100];
  if (num_packet){
    int len = wifiUdp.read(packetBuffer, num_packet);
    if (len > 0){ packetBuffer[len] = '\0'; }
    M5.Lcd.println(packetBuffer);
  }

}
