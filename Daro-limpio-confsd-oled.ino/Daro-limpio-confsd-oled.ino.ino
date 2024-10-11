#include <WiFi.h>
#include "time.h"
#include "DHT.h"
#include <U8g2lib.h>
#include <Wire.h>
#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <ESP32Time.h>
#define DHTPIN 4
#define DHTTYPE DHT11
#define CD_PIN 5
#include <ArduinoJson.hpp>
#include <ArduinoJson.h>
DHT dht(DHTPIN, DHTTYPE);
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);
ESP32Time rtc;

String lines[7];
int intw;
String ssid;
String pass;
String temp = "";
String hum = "";
int seg = 0;
int cnt = 0;
String ip;
String date;
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = -10800; // utc -3 bsas
const int   daylightOffset_sec = 0; // para horario de verano 3600

void setup() {
  dht.begin();
  Serial.begin(115200);
  u8g2.begin();
  if (!SD.begin(CD_PIN)){
    lines[0] = "sd err ";
    lines[1] = "Wifi err";
    dspbegin();
    return;
  }else{
    lines[0] = "sd ok.  ";
    leerconf();
    }
    Serial.printf("Connecting to %s ", ssid);
    WiFi.begin(ssid.c_str(), pass.c_str());
    while (WiFi.status() != WL_CONNECTED) {
      delay(100);
      Serial.print(".");
      intw = intw +1;
      if (intw == 50){
        break;
      }
    }
    if (WiFi.status() != WL_CONNECTED){
      lines[0] = lines[0] + "wifi err";
    }else{
      lines[0] = lines[0] + "wifi ok";
    }
    dspbegin();
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    struct tm timeinfo;
    if (getLocalTime(&timeinfo));{
      rtc.setTimeStruct(timeinfo);
    }
    IPAddress myIP = WiFi.localIP();
    ip = String() + WiFi.localIP()[0] + "." + WiFi.localIP()[1] + "." + WiFi.localIP()[2] + "." + WiFi.localIP()[3];
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    lines[5] = "Ip: " + ip;
    lines[6] = "mc:  " + WiFi.macAddress();
    Serial.println(rtc.getTime("%d/%m/%Y,%H:%M:%S") + "  Auto set rtf from ntp");
    WiFi.disconnect(true);// duuuuda duuuuda
    WiFi.mode(WIFI_OFF);//duuuda duuuda
    dspbegin();
}

void dspbegin(){
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.setCursor(2, 8);  // linea1
  u8g2.print(lines[0]); 
  u8g2.setCursor(2, 17); // linea2
  u8g2.print(lines[1]);
  u8g2.setCursor(2, 26); // linea3
  u8g2.print(lines[2]);
  u8g2.setCursor(2, 35); // linea4
  u8g2.print(lines[3]);
  u8g2.setCursor(2, 44); // linea5
  u8g2.print(lines[4]);
  u8g2.setCursor(2, 53); // linea6
  u8g2.print(lines[5]);
  u8g2.setCursor(2, 62); // linea7
  u8g2.print(lines[6]);
  u8g2.sendBuffer();
}
void dspdefault(){
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB14_tr);
  u8g2.setCursor(2, 15);
  u8g2.print("C    " + temp);
  u8g2.setCursor(2, 33);
  u8g2.print("%    " + hum);
  u8g2.setCursor(108, 15);
  u8g2.print("M");
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.setCursor(2, 53); // linea6
  u8g2.print(lines[5]);
  u8g2.setCursor(2, 62); // linea7
  u8g2.print(lines[6]);
  u8g2.sendBuffer();
}

void leerconf(){
  File file = SD.open("/config/config.txt", FILE_READ);
  StaticJsonDocument<256> doc;
  DeserializationError err = deserializeJson(doc, file);
  if (err) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(err.f_str());
    lines[0] = lines[0] + "cnf err";
  }
  ssid = doc["ssid"].as<String>();
  pass = doc["pass"].as<String>();
  lines[0] = lines[0] + "cnf ok  ";
  file.close();
}

void regsd(){
  File record = SD.open("/registros.txt", FILE_APPEND);
  if (record){
    record.println(temp + "," + hum + "," + rtc.getTime("%d/%m/%Y,%H:%M:%S"));
    record.close();
    lines[5] = "";
  }else{
    Serial.println("Fail record");
    lines[5] = "      SD Error      ";
  }
}

void animate(){
  int X;
  int Y;
  for(int x = 1; x<20; x++){
    X = random(5, 123);
    Y = random(5, 59);
    for(int r = 1; r<20; r++){
      u8g2.clearBuffer();
      u8g2.drawCircle(X, Y, r); //cernter 64.32.30
      u8g2.sendBuffer();
      //delay(1);
    }
  }    
}
void lineas(){
  u8g2.clearBuffer();
  for(int y =64; y>5; y--){
      if (y>5){
        u8g2.drawLine(63,64,63,y);
        u8g2.drawLine(64,64,64,y);
        u8g2.drawLine(65,64,65,y);
      }
      if (y>10){
        u8g2.drawLine(57,64,57,y);
        u8g2.drawLine(58,64,58,y);
        u8g2.drawLine(59,64,59,y);
        u8g2.drawLine(69,64,69,y);
        u8g2.drawLine(70,64,70,y);
        u8g2.drawLine(71,64,71,y);
      }
      if (y>15){
        u8g2.drawLine(75,64,75,y);
        u8g2.drawLine(76,64,76,y);
        u8g2.drawLine(77,64,77,y);
        u8g2.drawLine(51,64,51,y);
        u8g2.drawLine(52,64,52,y);
        u8g2.drawLine(53,64,53,y);
      }
      if (y>20){
        u8g2.drawLine(81,64,81,y);
        u8g2.drawLine(82,64,82,y);
        u8g2.drawLine(83,64,83,y);
        u8g2.drawLine(45,64,45,y);
        u8g2.drawLine(46,64,46,y);
        u8g2.drawLine(47,64,47,y);
      }
      if (y>25){
        u8g2.drawLine(87,64,87,y);
        u8g2.drawLine(88,64,88,y);
        u8g2.drawLine(89,64,89,y);
        u8g2.drawLine(38,64,38,y);
        u8g2.drawLine(39,64,39,y);
        u8g2.drawLine(40,64,40,y);
      }
      if (y>30){
        u8g2.drawLine(93,64,93,y);
        u8g2.drawLine(94,64,94,y);
        u8g2.drawLine(95,64,95,y);
        u8g2.drawLine(32,64,32,y);
        u8g2.drawLine(33,64,33,y);
        u8g2.drawLine(34,64,34,y);
      }
      if (y>35){
        u8g2.drawLine(99,64,99,y);
        u8g2.drawLine(100,64,100,y);
        u8g2.drawLine(101,64,101,y);
        u8g2.drawLine(26,64,26,y);
        u8g2.drawLine(27,64,27,y);
        u8g2.drawLine(28,64,28,y);
      }
      if (y>40){
        u8g2.drawLine(105,64,105,y);
        u8g2.drawLine(106,64,106,y);
        u8g2.drawLine(107,64,107,y);
        u8g2.drawLine(20,64,20,y);
        u8g2.drawLine(21,64,21,y);
        u8g2.drawLine(22,64,22,y);
      }
      if (y>45){
        u8g2.drawLine(111,64,111,y);
        u8g2.drawLine(112,64,112,y);
        u8g2.drawLine(113,64,113,y);
        u8g2.drawLine(14,64,14,y);
        u8g2.drawLine(15,64,15,y);
        u8g2.drawLine(16,64,16,y);
      }
    u8g2.sendBuffer();
  }
  u8g2.drawLine(1, 1, 1, 63);
  u8g2.drawLine(1, 1, 125, 1);
  u8g2.drawLine(125, 1, 125, 63);
  u8g2.sendBuffer();
  delay(2000);
}

void loop() {
  if (seg == 1 or seg == 0){
    temp = "";
    hum = "";
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    float hic = dht.computeHeatIndex(t, h, false);
    if (isnan(h) || isnan(t)) {
      Serial.println(F("Fail DHT11!"));
      lines[5] = "     dht11 error    ";
      return;
    }
    seg = 0;  
    temp.concat(t);
    hum.concat(h);
    seg = seg + 1;
    regsd();
  }
  //delay(100);
  //if (cnt == 10){
    dspdefault();
    //lineas();
    //cnt = 0;
  //}
  //cnt = cnt +1;
}
