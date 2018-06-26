#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

// WiFi Parameters
const char* ssid = "Ahn-Ji-young";
const char* password = "tellmeyouloveme";

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }
  pinMode(D7, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
}

void loop() {
  // Check WiFi Status
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;  //Object of class HTTPClient
    http.begin("http://techitvn.com/home/api/myhome.php?token=");
    int httpCode = http.GET();
    //Check the returning code                                                                  
    if (httpCode > 0) {
      // Parsing
      const size_t bufferSize = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(8) + 370;
      DynamicJsonBuffer jsonBuffer(bufferSize);
      JsonObject& root = jsonBuffer.parseObject(http.getString());
      root.printTo(Serial);
      // Parameters
      Serial.println("Json: "+http.getString());
      const char* token = root["token"];
      const char* light_1 = root["light_status"][0]["status"];
      // Output to serial monitor
      Serial.print("light_1:");
      Serial.println(light_1);

      char* cannot = "cannot access";
      char* expired = "expired token";
      char* off = "0";
      if(*token != *cannot && *token != *expired){
        if(*light_1 == *off){
          Serial.println("light_1 off");
          digitalWrite(D7, LOW);
        }else{
          Serial.println("light_1 on");
          digitalWrite(D7, HIGH);
        }
        
      }else{
        Serial.println("read err");
        digitalWrite(D7, HIGH);
        delay(2000);
        digitalWrite(D7, LOW);
      }
    }
    http.end();   //Close connection
  }
  // Delay
  delay(1000);
}
