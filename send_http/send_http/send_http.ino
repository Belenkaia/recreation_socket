/**
   Authorization.ino

    Created on: 09.12.2015

*/

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#include <WiFiClient.h>
#define SERVER_IP "192.168.1.64:8080"
ESP8266WiFiMulti WiFiMulti;
int led1 = 12;
int led2 = 13;
int led3 = 5;
int led4 = 4;
int ledOut = 7;
int countFreeSocket;
void setup() {
  countFreeSocket = 0;
  pinMode(led1, INPUT);
  //pinMode(ledOut, OUTPUT);
  pinMode(led2, INPUT);
  pinMode(led3, INPUT);
  pinMode(led4, INPUT);
  Serial.begin(115200);
  // Serial.setDebugOutput(true);

  Serial.println();
  Serial.println();
  Serial.println();

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  WiFi.mode(WIFI_STA);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    WiFi.begin("HackSpace", "hackspace");
  
  }
  IPAddress local_ip = WiFi.localIP();
  Serial.print("IP address: ");
  Serial.println(local_ip);
    
}

void loop() {
  //digitalWrite(ledOut, HIGH);
  countFreeSocket = 0;
  if(digitalRead(led4) != 1)
  {
    countFreeSocket ++;
  }
    if(digitalRead(led3) != 1)
  {
    countFreeSocket ++;
  }
    if(digitalRead(led2) != 1)
  {
    countFreeSocket ++;
  }
    if(digitalRead(led1) != 1)
  {
    countFreeSocket ++;
  }
  Serial.print("count free socket: ");
  Serial.println(countFreeSocket);
  delay(5000);
     if ((WiFi.status() == WL_CONNECTED)) {

    WiFiClient client;
    HTTPClient http;

    Serial.print("[HTTP] begin...\n");
    // configure traged server and url
    String httpStr = "http://";
    String http2Str = "/recreation/api/data";
    http.begin(client, httpStr+SERVER_IP+http2Str); //HTTP
    http.addHeader("Content-Type", "application/json");

    Serial.print("[HTTP] POST...\n");
    // start connection and send HTTP header and body
    String firstStr = "{\"deviceId\" : 1,\n \"placeId\" : 12,\n\"count\" : ";
    String secondStr = "}";
    String postString =  firstStr +  countFreeSocket + secondStr; 
    int httpCode = http.POST(postString);

    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] POST... code: %d\n", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK) {
        const String& payload = http.getString();
        Serial.println("received payload:\n<<");
        Serial.println(payload);
        Serial.println(">>");
      }
    } else {
      Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  }

  delay(10000);

}
