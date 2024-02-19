//Board:NodeMCU 1.0 (ESP-12E Module)
#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#include <WiFiClient.h>

#include <ArduinoJson.h>
#include "Nextion.h"

int Relay1;
int Relay2;

#define RELAY_ON 1  // GPIO value to write to turn on attached relay
#define RELAY_OFF 0 // GPIO value to write to turn off attached relay
#define RELAY1_PIN D0
#define RELAY2_PIN D1

ESP8266WiFiMulti WiFiMulti;
char  PageName[6];
float PageTemp[6];
float PageDayTemp[6];
float PageNightTemp[6];
float PageOutTemp[6];
int PageDayIdx[6];
int PageNightIdx[6];
int PageOutIdx[6];
int PageStatus[6];
int ThermostatMode;
int ThermostatModeIdx;
int BoilerStatus;
int CurrentPage=0;
int CurrentRoom=0;

float tmpDayTemp;
float tmpNightTemp;
float tmpOutTemp;

int WIFIpic = 0;


// Nextion
NexText message = NexText(0, 2, "message");

NexPage page0 = NexPage(0, 0, "page0");
NexPage page1 = NexPage(1, 0, "page1");
NexPage page2 = NexPage(2, 0, "page2");

NexText cTemp = NexText(1,5,"Homerseklet");
NexText cName = NexText(1,1,"t0");

NexButton b0 = NexButton(1, 2, "b0");
NexButton b1 = NexButton(1, 3, "b1");

NexDSButton bt0 = NexDSButton(1, 8, "bt0");
NexDSButton bt1 = NexDSButton(1, 9, "bt1");
NexDSButton bt2 = NexDSButton(1, 10, "bt2");
NexDSButton bt3 = NexDSButton(1, 11, "bt3");

NexHotspot m0 = NexHotspot(1,22,"m0");
NexHotspot m1 = NexHotspot(1,23,"m1");

NexButton b3 = NexButton(2, 10, "b3");
NexButton b4 = NexButton(2, 11, "b4");
NexButton b5 = NexButton(2, 12, "b5");
NexButton b6 = NexButton(2, 13, "b6");
NexButton b7 = NexButton(2, 14, "b7");
NexButton b8 = NexButton(2, 15, "b8");
NexButton bs = NexButton(2, 3, "b2");
NexTouch *nex_listen_list[] = 
{
    &page0,
    &page1,
    &page2,
    &b0,
    &b1,
    &bt0,
    &bt1,
    &bt2,
    &bt3,
    &m0,
    &m1,
    &b3,
    &b4,
    &b5,
    &b6,
    &b7,
    &b8,
    &bs,
    NULL
};

void page0PushCallback(void *ptr)
{
    CurrentPage=0;

  if ((WiFiMulti.run() == WL_CONNECTED)) {
    message.setText("");
    Serial.print("p0.pic=1");
    Serial.write(0xFF);
    Serial.write(0xFF);
    Serial.write(0xFF);
  } else {
    message.setText("WIFI Error");
    Serial.print("p0.pic=0");
    Serial.write(0xFF);
    Serial.write(0xFF);
    Serial.write(0xFF);
  }    
}

void page1PushCallback(void *ptr)
{
    CurrentPage=1;
    displaydata();
    getDomData();
}

void page2PushCallback(void *ptr)
{
    CurrentPage=2;
    tmpDayTemp=PageDayTemp[CurrentRoom];
    tmpNightTemp=PageNightTemp[CurrentRoom];
    tmpOutTemp=PageOutTemp[CurrentRoom];
    displayThermostat();
}

void b6PushCallback(void *ptr)
{
  tmpDayTemp-=0.5;
  displayThermostat();
}

void b3PushCallback(void *ptr)
{
  tmpDayTemp+=0.5;
  displayThermostat();
}
void b7PushCallback(void *ptr)
{
  tmpNightTemp-=0.5;
  displayThermostat();
}

void b4PushCallback(void *ptr)
{
  tmpNightTemp+=0.5;
  displayThermostat();
}
void b8PushCallback(void *ptr)
{
  tmpOutTemp-=0.5;
  displayThermostat();
}

void b5PushCallback(void *ptr)
{
  tmpOutTemp+=0.5;
  displayThermostat();
}

void SavePushCallback(void *ptr)
{
    PageDayTemp[CurrentRoom]=tmpDayTemp;
    PageNightTemp[CurrentRoom]=tmpNightTemp;
    PageOutTemp[CurrentRoom]=tmpOutTemp;
    saveHVAC(PageDayIdx[CurrentRoom],tmpDayTemp);
    saveHVAC(PageNightIdx[CurrentRoom],tmpNightTemp);
    saveHVAC(PageOutIdx[CurrentRoom],tmpOutTemp);
    
}
void PrevRoomPushCallback(void *ptr)
{
    if (CurrentRoom>0){
      CurrentRoom--;
      displaydata();
    }
}

void NextRoomPushCallback(void *ptr)
{
    if (CurrentRoom<5){
      CurrentRoom++;
      displaydata();
    }
}

void OffPushCallback(void *ptr)
{
  ThermostatMode=0;
  setMode();
  displaydata();
}
void DayPushCallback(void *ptr)
{
  ThermostatMode=10;
  setMode();
  displaydata();
}
void NightPushCallback(void *ptr)
{
  ThermostatMode=20;
  setMode();
  displaydata();
}
void OutPushCallback(void *ptr)
{
  ThermostatMode=30;
  setMode();
  displaydata();
}

void WIFIPushCallback(void *ptr)
{
  displaydata();
}

void RefreshPushCallback(void *ptr)
{
  getDomData();
}
void setup() {

  pinMode(RELAY1_PIN, OUTPUT);
  digitalWrite(RELAY1_PIN, LOW);
  pinMode(RELAY2_PIN, OUTPUT);
  digitalWrite(RELAY2_PIN, LOW);

  
  Serial.begin(9600);
  delay(500);
  Serial.print("baud=115200");
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.end();
  Serial.begin(115200);
  nexInit();
  page0.attachPush(page0PushCallback);
  page1.attachPush(page1PushCallback);
  page2.attachPush(page2PushCallback);
  b0.attachPush(PrevRoomPushCallback);
  b1.attachPush(NextRoomPushCallback);
  bt0.attachPush(OffPushCallback);
  bt1.attachPush(DayPushCallback);
  bt2.attachPush(NightPushCallback);
  bt3.attachPush(OutPushCallback);
  m1.attachPush(RefreshPushCallback);
  
  b3.attachPush(b3PushCallback);
  b4.attachPush(b4PushCallback);
  b5.attachPush(b5PushCallback);
  b6.attachPush(b6PushCallback);
  b7.attachPush(b7PushCallback);
  b8.attachPush(b8PushCallback);
  bs.attachPush(SavePushCallback);
  
  //message.setText("Connecting to WIFI.");
  // Serial.setDebugOutput(true);

  //Serial.println();
  //Serial.println();
  //Serial.println();


  for (uint8_t t = 4; t > 0; t--) {
    //Serial.printf("[SETUP] WAIT %d...\n", t);
    //Serial.flush();
    Serial.print("message.txt=");
    Serial.print("va1.txt+\"");
    Serial.print(t);
    Serial.print("\"");
    Serial.write(0xFF);
    Serial.write(0xFF);
    Serial.write(0xFF);
    delay(1000);
  }
  Serial.print("message.txt=");
  Serial.print("va0.txt");
  Serial.write(0xFF);
  Serial.write(0xFF);
  Serial.write(0xFF);
  Serial.print("p0.pic=2");
  Serial.write(0xFF);
  Serial.write(0xFF);
  Serial.write(0xFF);  
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("TAIKAHOME", "Kukur1kUTaika");
  if ((WiFiMulti.run() == WL_CONNECTED)) {
    message.setText("");
    WIFIpic=1;
  } else {
    message.setText("Hiba");
    WIFIpic=0;
    //init Thermostat Data
    for (int t = 0; t < 6; t++) {
      //PageName[t]="";
      PageTemp[t]=22.1+t;
      PageDayTemp[t]=21+t;
      PageNightTemp[t]=18+t;
      PageOutTemp[t]=17+t;
      PageDayIdx[t]=2;
      PageNightIdx[t]=3;
      PageOutIdx[t]=4;
      PageStatus[t]=1;
    }
    ThermostatMode=10;
    ThermostatModeIdx=1;
    BoilerStatus=1;
  }
  Serial.print("p0.pic=");
  Serial.print(WIFIpic);
  Serial.write(0xFF);
  Serial.write(0xFF);
  Serial.write(0xFF);  
}

void loop() {

  nexLoop(nex_listen_list);
}
void getDomData(){
const size_t capacity = JSON_ARRAY_SIZE(7) + JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(4) + 6*JSON_OBJECT_SIZE(10) + 530;
DynamicJsonBuffer jsonBuffer(capacity);
  // wait for WiFi connection
  if ((WiFiMulti.run() == WL_CONNECTED)) {

    WiFiClient client;

    HTTPClient http;
    Serial.print("p0.pic=2");
    Serial.write(0xFF);
    Serial.write(0xFF);
    Serial.write(0xFF);
    //Serial.print("[HTTP] begin...\n");
    message.setText("");
    if (http.begin(client, "http://192.168.0.200:8080/thermostat.json")) {  // HTTP


      //Serial.print("[HTTP] GET...\n");
      // start connection and send HTTP header
      int httpCode = http.GET();

      // httpCode will be negative on error
      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        //Serial.printf("[HTTP] GET... code: %d\n", httpCode);

        // file found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = http.getString();
          //Serial.println(payload);
          JsonObject& doc = jsonBuffer.parseObject(payload);
          //JsonArray& result_doc = doc["result"];
          // általános adatok
          JsonObject& result = doc["result"][0];
          ThermostatModeIdx = result["modeIdx"]; // 0
          ThermostatMode = result["mode"]; // 30
          BoilerStatus=0;
          if (result["status"] == "On") {
            BoilerStatus=1;
          }
          Relay1=LOW;
          Relay2=LOW;
          for (uint8_t t = 1; t < 7; t++) {
            JsonObject& result = doc["result"][t];
            PageName[t-1]=result["name"];
            //Serial.printf(result["name"]);
            PageTemp[t-1]=result["temp"];
            PageDayTemp[t-1]=result["dtemp"];
            PageNightTemp[t-1]=result["ntemp"];
            PageOutTemp[t-1]=result["otemp"];
            PageDayIdx[t-1]=result["didx"];
            PageNightIdx[t-1]=result["nidx"];
            PageOutIdx[t-1]=result["oidx"];
            PageStatus[t-1]=0;
            if (result["status"] == "On") {
              PageStatus[t-1]=1;
              if (t==1) {
                Relay1=HIGH;
              } else {
                Relay2=HIGH;
              }
            }
          }
        }
        if( CurrentPage==1){
          displaydata();
        }
        Serial.print("p0.pic=");
        Serial.print(WIFIpic);
        Serial.write(0xFF);
        Serial.write(0xFF);
        Serial.write(0xFF);

      //} else {
        //Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }
      digitalWrite(RELAY1_PIN, Relay1);
      digitalWrite(RELAY2_PIN, Relay2);
      //digitalWrite(RELAY2_PIN, Relay1);
      http.end();
    //} else {
      //Serial.printf("[HTTP} Unable to connect\n");
    }
  }

}

void displaydata(){
  if (CurrentPage==1){
        Serial.print("Homerseklet.txt=");
        Serial.print("\"");
        Serial.print(PageTemp[CurrentRoom],1);        
        Serial.print("\"");
        Serial.print("+cons6.txt");
        Serial.write(0xFF);
        Serial.write(0xFF);
        Serial.write(0xFF);        
        Serial.print("t0.txt=");
        Serial.print("cons");
        Serial.print(CurrentRoom);
        Serial.print(".txt");
        Serial.write(0xFF);
        Serial.write(0xFF);
        Serial.write(0xFF);
        if (ThermostatMode==0){
          Serial.print("bt0.val=1");
          Serial.write(0xFF);
          Serial.write(0xFF);
          Serial.write(0xFF);
          Serial.print("t2.txt=\"------\"");
          Serial.write(0xFF);
          Serial.write(0xFF);
          Serial.write(0xFF);          
        } else {
          Serial.print("bt0.val=0");
          Serial.write(0xFF);
          Serial.write(0xFF);
          Serial.write(0xFF);          
        }
        if (ThermostatMode==10){
          Serial.print("bt1.val=1");
          Serial.write(0xFF);
          Serial.write(0xFF);
          Serial.write(0xFF);
          Serial.print("t2.txt=\"");
          Serial.print(PageDayTemp[CurrentRoom],1);
          Serial.print("\"+cons6.txt");
          Serial.write(0xFF);
          Serial.write(0xFF);
          Serial.write(0xFF);
        } else {
          Serial.print("bt1.val=0");
          Serial.write(0xFF);
          Serial.write(0xFF);
          Serial.write(0xFF);          
        }
        if (ThermostatMode==20){
          Serial.print("bt2.val=1");
          Serial.write(0xFF);
          Serial.write(0xFF);
          Serial.write(0xFF);
          Serial.print("t2.txt=\"");
          Serial.print(PageNightTemp[CurrentRoom],1);
          Serial.print("\"+cons6.txt");
          Serial.write(0xFF);
          Serial.write(0xFF);
          Serial.write(0xFF);
        } else {
          Serial.print("bt2.val=0");
          Serial.write(0xFF);
          Serial.write(0xFF);
          Serial.write(0xFF);          
        }
         if (ThermostatMode==30){
          Serial.print("bt3.val=1");
          Serial.write(0xFF);
          Serial.write(0xFF);
          Serial.write(0xFF);
          Serial.print("t2.txt=\"");
          Serial.print(PageOutTemp[CurrentRoom],1);
          Serial.print("\"+cons6.txt");
          Serial.write(0xFF);
          Serial.write(0xFF);
          Serial.write(0xFF);
        } else {
          Serial.print("bt3.val=0");
          Serial.write(0xFF);
          Serial.write(0xFF);
          Serial.write(0xFF);          
        }
        Serial.print("p1.pic=");
        Serial.print(PageStatus[CurrentRoom]+5);
        Serial.write(0xFF);
        Serial.write(0xFF);
        Serial.write(0xFF);
        Serial.print("p2.pic=");
        Serial.print(BoilerStatus+3);
        Serial.write(0xFF);
        Serial.write(0xFF);
        Serial.write(0xFF);        
    }
  
  }

void setMode(){

  if ((WiFiMulti.run() == WL_CONNECTED)) {

    WiFiClient client;

    HTTPClient http;
    Serial.print("p0.pic=2");
    Serial.write(0xFF);
    Serial.write(0xFF);
    Serial.write(0xFF);
    String wUrl1="http://192.168.0.200:8080/json.htm?type=command&param=switchlight&idx=37";
    String wURL2="&switchcmd=Set%20Level&level=";
    String wIDX= String(ThermostatModeIdx);
    String wMode= String(ThermostatMode);
    String wURL=wUrl1+wURL2+wMode;
    if (http.begin(client, wURL)) {  // HTTP
      int httpCode = http.GET();
      if (httpCode > 0) {

        displaydata();
      }
      http.end();
      Serial.print("p0.pic=");
      Serial.print(WIFIpic);
      Serial.write(0xFF);
      Serial.write(0xFF);
      Serial.write(0xFF);
    }
  }
}
void displayThermostat(){
        Serial.print("t0.txt=");
        Serial.print("cons");
        Serial.print(CurrentRoom);
        Serial.print(".txt");
        Serial.write(0xFF);
        Serial.write(0xFF);
        Serial.write(0xFF);

        
        Serial.print("t2.txt=");
        Serial.print("\"");
        Serial.print(tmpDayTemp,1);        
        Serial.print("\"");
        Serial.print("+cons6.txt");
        Serial.write(0xFF);
        Serial.write(0xFF);
        Serial.write(0xFF); 
        Serial.print("t6.txt=");
        Serial.print("\"");
        Serial.print(tmpNightTemp,1);        
        Serial.print("\"");
        Serial.print("+cons6.txt");
        Serial.write(0xFF);
        Serial.write(0xFF);
        Serial.write(0xFF);
        Serial.print("t7.txt=");
        Serial.print("\"");
        Serial.print(tmpOutTemp,1);        
        Serial.print("\"");
        Serial.print("+cons6.txt");
        Serial.write(0xFF);
        Serial.write(0xFF);
        Serial.write(0xFF);        
}

void saveHVAC(int hvacIDX,float hvacTemp){
  if ((WiFiMulti.run() == WL_CONNECTED)) {

    WiFiClient client;

    HTTPClient http;
    Serial.print("p0.pic=2");
    Serial.write(0xFF);
    Serial.write(0xFF);
    Serial.write(0xFF);
    String wUrl1="http://192.168.0.200:8080/json.htm?type=command&param=setsetpoint&idx=";
    String wURL2="&setpoint=";
    String wIDX= String(hvacIDX);
    String wValue= String(hvacTemp);
    String wURL=wUrl1+wIDX+wURL2+wValue;
    if (http.begin(client, wURL)) {  // HTTP
      int httpCode = http.GET();
      if (httpCode > 0) {

        displaydata();
      }
      http.end();
      Serial.print("p0.pic=");
      Serial.print(WIFIpic);
      Serial.write(0xFF);
      Serial.write(0xFF);
      Serial.write(0xFF);
    }
  }
}
