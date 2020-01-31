
#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#include <WiFiClient.h>

ESP8266WiFiMulti WiFiMulti;
int led1 = 12;
int led2 = 13;
int led3 = 5;
int led4 = 4;
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
   
}
