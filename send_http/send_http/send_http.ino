#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
//for leds
#define NUM_LEDS 22
#include "FastLED.h"
#define PIN 14
//
#define SERVER_IP "35.228.181.111"//"192.168.1.64:8080"
ESP8266WiFiMulti WiFiMulti;
CRGB leds[NUM_LEDS];

int led1 = 12;
int led2 = 13;
int led3 = 5;
int led4 = 4;
int ledOut = 7;
int delay_count = 0;
    // for leds
  int red = 100;
  int green = 100; 
  int blue = 100;
int countFreeSocket;
int lastStatisticSocket[4];
int lastSendedSocket;
int somethingChanged = 1;
void setup() {
  //for leds
  FastLED.addLeds<WS2811, PIN, GRB>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(50);
  pinMode(PIN, OUTPUT);
  //
  countFreeSocket = 0;
  lastSendedSocket = 1;
  
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

  
  if(digitalRead(led4) != 1)
  {
    countFreeSocket ++;
    green = 255;
    red = 0;
    lastStatisticSocket[0] = 0;
    //lastSendedSocket[0] = 1;
  }else
  {
    lastStatisticSocket[0] = 1;
    //lastSendedSocket[0] = 0;
    green = 0;
    red = 255;
  }
  for (int i = 0; i < 5; i++ ) {         // от 0 до первой трети
    leds[i] = CRGB(red, green, blue);  // HSV. Увеличивать HUE (цвет)
    // умножение i уменьшает шаг радуги
  }
    if(digitalRead(led3) != 1)
  {
    //lastSendedSocket[1] = 1;
    lastStatisticSocket[1] = 0;
    countFreeSocket ++;
    green = 255;
    red = 0;
  }else
  {
    //lastSendedSocket[1] = 0;
    lastStatisticSocket[1] = 1;
    green = 0;
    red = 255;
  }
  for (int i = 5; i < 11; i++ ) {         // от 0 до первой трети
    leds[i] = CRGB(red, green, blue);  // HSV. Увеличивать HUE (цвет)
    // умножение i уменьшает шаг радуги
  }
    if(digitalRead(led2) != 1)
  {
     //lastSendedSocket[2] = 1;
     lastStatisticSocket[2] = 0;
     countFreeSocket ++;
     green = 255;
    red = 0;
  }else
  {
    //lastSendedSocket[2] = 0;
    lastStatisticSocket[2] = 1;
    green = 0;
    red = 255;
  }
   for (int i = 11; i < 16; i++ ) {         // от 0 до первой трети
    leds[i] = CRGB(red, green, blue);  // HSV. Увеличивать HUE (цвет)
    // умножение i уменьшает шаг радуги
  }
    if(digitalRead(led1) != 1)
  {
    //lastSendedSocket[3] = 1;
    lastStatisticSocket[3] = 0;
    countFreeSocket ++;
    green = 255;
    red = 0;
  }else
  {
    //lastSendedSocket[3] = 0;
    lastStatisticSocket[3] = 1;
    green = 0;
    red = 255;
  }
    for (int i = 16; i < 22; i++ ) {
    leds[i] = CRGB(red, green, blue);
  }
  FastLED.show();
}

void loop() {
  //countFreeSocket = 0;
  if(digitalRead(led4) != lastStatisticSocket[0])
  {
    lastStatisticSocket[0] = digitalRead(led4);
    if(lastStatisticSocket[0] == 0)
    {
      countFreeSocket ++;
      green = 255;
      red = 0;
    }
    else
    {
      countFreeSocket --;
      green = 0;
      red = 255;
    }
    for (int i = 0; i < 5; i++ ) {
      leds[i] = CRGB(red, green, blue);
    }
  }
  
  if(digitalRead(led3) != lastStatisticSocket[1])
  {
    lastStatisticSocket[1] = digitalRead(led3);
    if(lastStatisticSocket[1] == 0)
    {
      countFreeSocket ++;
      green = 255;
      red = 0;
    }
    else
    {
      countFreeSocket --;
      green = 0;
      red = 255;
    }
    for (int i = 5; i < 11; i++ ) {
      leds[i] = CRGB(red, green, blue);
    }
  }

if(digitalRead(led2) != lastStatisticSocket[2])
  {
    lastStatisticSocket[2] = digitalRead(led2);
    if(lastStatisticSocket[2] == 0)
    {
      countFreeSocket ++;
      green = 255;
      red = 0;
    }
    else
    {
      countFreeSocket --;
      green = 0;
      red = 255;
    }
   for (int i = 11; i < 16; i++ ) {
    leds[i] = CRGB(red, green, blue);
   }
  }
  
  if(digitalRead(led1) != lastStatisticSocket[3])
  {
    lastStatisticSocket[3] = digitalRead(led1);
    if(lastStatisticSocket[3] == 0)
    {
      countFreeSocket ++;
      green = 255;
      red = 0;
    }
    else
    {
      countFreeSocket --;
      green = 0;
      red = 255;
    }
     for (int i = 16; i < 22; i++ ) {
      leds[i] = CRGB(red, green, blue);
    }
  }

   //somethingChanged = 0;
   //for(int i = 0; i < 4; i ++){
   // if(lastSendedSocket[i] != lastStatisticSocket[i])
   //   somethingChanged = 1; 
    //}

  FastLED.show();
  
//  Serial.print("count free socket: ");
//  Serial.println(countFreeSocket);
  //delay(1000);
  delay_count ++;
  
if ((WiFi.status() == WL_CONNECTED)) {
  if(/*(delay_count == 10) &&*/(lastSendedSocket != countFreeSocket)){
    lastSendedSocket = countFreeSocket;
    delay_count = 0;
    //for(int i = 0; i < 4; i ++){
    //lastSendedSocket[i] = lastStatisticSocket[i]; 
    //}
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
 }

  //
  delay(100);

}
