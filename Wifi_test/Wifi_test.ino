#include <M5Stack.h>
#include <WiFi.h>

#include <Ethernet.h>

//reference = https://msr-r.net/m5stack-wifi/


const char* ssid     = "aterm-a5535b-g"; //"GlocalNet_0HG8TN"; //"aterm-a5535b-g";
const char* password = "3f4842c397e8f"; //"68866409";  //"3f4842c397e8f";
const uint16_t port = 50007;
const char * host = "192.168.0.6"; // ip or dns
//https://qiita.com/ufoo68/items/2c5a2d7e427b46ff0c88

WiFiClient client;


void setup() {
  M5.begin();
  M5.Power.begin();
  M5.Lcd.setTextSize(2);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    M5.Lcd.print(".");
  }
  M5.Lcd.print("\r\nWiFi connected\r\nIP address: ");
  M5.Lcd.println(WiFi.localIP());

  M5.Lcd.println("Connecting to PC.....");
  while(!client.connect(host, port)) {
    delay(500);
  }
  M5.Lcd.println("Connect success!!");
  M5.Lcd.println("Main process srarted!");
}

void loop() {
  client.println("Hello_World");
  
//  unsigned long timeout = millis();
//  while (client.available() == 0) {
//    if (millis() - timeout > 5000) {
//      M5.Lcd.println(">>> Client Timeout !");
//      client.stop();
//      return;
//    }
//  }
//  while(client.available()){
//    String line = client.readStringUntil('\r');
//    M5.Lcd.print(line);
//  }
  delay(1000);
}
  




//
//void loop() {
//  // listen for incoming clients
//  EthernetClient client = server.available();
//  if (client) {
//    Serial.println("new client");
//    // an http request ends with a blank line
//    boolean currentLineIsBlank = true;
//    while (client.connected()) {
//      if (client.available()) {
//        char c = client.read();
//        Serial.write(c);
//        if (c=='1')
//          digitalWrite(9,HIGH);
//        else if (c=='0')
//          digitalWrite(9, LOW);
//        else if (c=='\n' || c=='\r')
//          continue;
//        else
//          break;
//
//      }
//    }
//    // give the web browser time to receive the data
//    delay(1);
//    // close the connection:
//    client.stop();
//    Serial.println("client disconnected");
//  }
//}
