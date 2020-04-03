#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
//for leds
#define NUM_LEDS 22
#include "FastLED.h";
#define PIN 14
#define SERVER_IP "35.228.181.111" //"192.168.1.64:8080"//"35.228.181.111"//"192.168.1.64:8080"
ESP8266WiFiMulti WiFiMulti;
CRGB leds[NUM_LEDS];
String WIFI_NAME = "Rostelecom116";//"HackSpace";
String WIFI_KEY = "0803384429";//"hackspace";
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

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void initLeds()
{
  //for leds
  FastLED.addLeds<WS2811, PIN, GRB>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(50);
  pinMode(PIN, OUTPUT);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void initSocketDetectors()
{
  pinMode(led1, INPUT);
  pinMode(led2, INPUT);
  pinMode(led3, INPUT);
  pinMode(led4, INPUT);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void wait()
{
  Serial.println();
  Serial.println();
  Serial.println();
  for (uint8_t t = 4; t > 0; t--) 
  {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void startWIFIConnection()
{
   WiFi.mode(WIFI_STA);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    WiFi.begin(WIFI_NAME, WIFI_KEY);
  }
  IPAddress local_ip = WiFi.localIP();
  Serial.print("IP address: ");
  Serial.println(local_ip);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void checkSocket(int led, int indexStatistic, int ledFirstIndex, int ledLastIndex)
{
   if(digitalRead(led) != 1)
  {
    countFreeSocket ++;
    green = 255;
    red = 0;
    lastStatisticSocket[indexStatistic] = 0;
  }else
  {
    lastStatisticSocket[indexStatistic] = 1;
    green = 0;
    red = 255;
  }
  for (int i = ledFirstIndex; i < ledLastIndex; i++ ) // colored part of LED which associated with that socket
  {
    leds[i] = CRGB(red, green, blue);
  }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void checkSocketChange(int led, int indexStatistic, int ledFirstIndex, int ledLastIndex)
{
  if(digitalRead(led) != lastStatisticSocket[indexStatistic])
  {
    lastStatisticSocket[indexStatistic] = digitalRead(led);
    if(lastStatisticSocket[indexStatistic] == 0)
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
    for (int i = ledFirstIndex; i < ledLastIndex; i++ ) {
      leds[i] = CRGB(red, green, blue);
    }
  }
}


//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void sendHTTP()
{
  if ((WiFi.status() == WL_CONNECTED)) {
  if(/*(delay_count == 10) &&*/(lastSendedSocket != countFreeSocket)){
    lastSendedSocket = countFreeSocket;
    //  delay_count = 0;
    //for(int i = 0; i < 4; i ++){
    //lastSendedSocket[i] = lastStatisticSocket[i]; 
    //}
    WiFiClient client;
    HTTPClient http;

    Serial.print("[HTTP] begin...\n");
    // configure traged server and url
    //String httpStr = "http://";
    //String http2Str = "/recreation/api/data";
    http.begin(client, "http://35.228.181.111/");///recreation/api/data");// httpStr+SERVER_IP+http2Str); //HTTP
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
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void setup() {
  initLeds();
  countFreeSocket = 0;
  lastSendedSocket = 1;
  
  initSocketDetectors();
  Serial.begin(115200);
  wait();
  startWIFIConnection();
//checkSocket(int led, int indexStatistic, int ledFirstIndex, int ledLastIndex)

  checkSocket(led4, 0, 0, 5);
  checkSocket(led3, 1, 5, 11);
 
  //checkSocket(int led, int indexStatistic, int ledFirstIndex, int ledLastIndex)
  checkSocket(led2, 2, 11, 16);
  checkSocket(led1, 3, 16, 22);
  FastLED.show();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void loop() {
   checkSocketChange(led4, 0, 0, 5);
   checkSocketChange(led3, 1, 5, 11);
   checkSocketChange(led2, 2, 11, 16);
   checkSocketChange(led1, 3, 16, 22);

  FastLED.show();
  
  //Serial.print("count free socket: ");
  //Serial.println(countFreeSocket);
  //delay(1000);
  //delay_count ++;
  sendHTTP();

  //
  delay(100);

}
