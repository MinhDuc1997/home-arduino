
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include <exception>

const char* ssid = "Ahn-Ji-young";
const char* password = "";

const char* host = "techitvn.com";
const char* url = "/home/api/myhome.php?token=";
const int httpsPort = 443;

// SHA1 fingerprint of the certificate
const char* fingerprint = "B5 9C 28 8F BC 43 66 9D D0 7F 00 FA 50 6E ED 39 11 64 6A CE";

String line;
int err = 0;
int check = 0;

void setup() {
    Serial.begin(9600);
    Serial.println();
    Serial.print("connecting to ");
    Serial.println(ssid);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    pinMode(D1, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
}

JsonObject& readData(){
    int connectHost = 0;
    int verifySsl = 0;
    if(WiFi.status() == WL_CONNECTED){
      Serial.println("WiFi connected");
      Serial.println("IP address: ");
      Serial.println(WiFi.localIP());
    
      // Use WiFiClientSecure class to create TLS connection
      WiFiClientSecure client;
      Serial.print("connecting to ");
      Serial.println(host);
      if (!client.connect(host, httpsPort)) {
        err++;
        Serial.println("connect to host failed");
      }else{
          Serial.println("connected to host");
          connectHost = 1;
      }
    
      if (connectHost == 1 && client.verify(fingerprint, host)) {
        Serial.println("certificate matches");
        verifySsl = 1;
      } else {
        Serial.println("certificate doesn't match");
      }
  
      if(verifySsl == 1){
        client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                   "Host: " + host + "\r\n" +
                   "User-Agent: BuildFailureDetectorESP8266\r\n" +
                   "Connection: close\r\n\r\n");
    
        Serial.println("request sent");
        while (client.connected()) {
          if (client.readStringUntil('\n') == "\r") {
            Serial.println("headers received");
            break;
          }
        }
        line = client.readStringUntil('}');
        line = line + "}]}";
        Serial.println(line);
        const size_t bufferSize = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(8) + 370;
        DynamicJsonBuffer jsonBuffer(bufferSize);
        JsonObject& root = jsonBuffer.parseObject(line);
        return root;
      }
    }else{
      err++;
    }
}

void run(JsonObject& root){
    const char* token = root["token"];
      const char* light_1 = root["light_status"][0]["status"];
      // Output to serial monitor

      char* cannot = "cannot access";
      char* expired = "expired token";
      char* off = "0";
      if(*token != *cannot && *token != *expired){
        if(*light_1 == *off){
          Serial.println("light_1 on");
          digitalWrite(D1, LOW);
        }else{
          Serial.println("light_1 off");
          digitalWrite(D1, HIGH);
        }
        
      }
}

void loop() {
    if(err >= 5){
      if(check == 0){
        ssid = "mducc";
        check = 1;
      }else{
        ssid = "Ahn-Ji-young";
        check = 0; 
      }
      err = 0;
      setup();
    }
    __try{
        run(readData());
    }__catch(const std::exception& e){ }
    delay(1000);
}
 
