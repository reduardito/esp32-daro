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
DHT dht(DHTPIN, DHTTYPE);
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);
ESP32Time rtc;

String sd;
String temp = "";
String hum = "";
int seg = 0;
String ip;
String date;
const char* ssid       = "clarowifi212";
const char* password   = "11223344";
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = -10800; // utc -3 bsas
const int   daylightOffset_sec = 0; // para horario de verano 3600

void setup()
{
  Serial.begin(115200);
  Serial.println("");
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println(" CONNECTED");
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  struct tm timeinfo;
  if (getLocalTime(&timeinfo));{
    rtc.setTimeStruct(timeinfo);
  }
  IPAddress myIP = WiFi.localIP();
  Serial.print("IP: ");
  Serial.println(myIP);
  ip = String() + WiFi.localIP()[0] + "." + WiFi.localIP()[1] + "." + WiFi.localIP()[2] + "." + WiFi.localIP()[3];
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  u8g2.begin();
  dht.begin();
  if (!SD.begin(CD_PIN)){
    Serial.println("Fail SD Card");
    return;
  }
}

void loop()
{
  delay(1000);
  //if (!SD.begin(CD_PIN)){
    //sd = "SD Error";
  //}else{
    //sd = "SD Ok";
  //}
  if (seg == 10 or seg == 0){
    seg = 0;
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    float hic = dht.computeHeatIndex(t, h, false);  
    Serial.print(F("Humedad: "));
    temp.concat(t);
    hum.concat(h);
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_ncenB14_tr);
    u8g2.setCursor(2, 15);
    u8g2.print("C    " + temp);
    u8g2.setCursor(2, 33);
    u8g2.print("%    " + hum);
    u8g2.setCursor(108, 15);
    u8g2.print("M");
    u8g2.setFont(u8g2_font_ncenB08_tr);
    u8g2.setCursor(2, 50);
    u8g2.print("mc:  " + WiFi.macAddress());                  
    if (isnan(h) || isnan(t)) {
      Serial.println(F("Fail DHT11!"));
      return;
    }
    Serial.print(h);
    Serial.print(F("%  Temperatura: "));
    Serial.print(t);
    Serial.print(F("°C "));
    Serial.println(rtc.getTime("%d/%m/%Y %H:%M:%S"));
    File record = SD.open("/registros.txt", FILE_APPEND);
    if (record){
      record.println(temp + "," + hum + "," + rtc.getTime("%d/%m/%Y,%H:%M:%S"));
      record.close();
      sd = "SD Ok";
    }else{
      Serial.println("Fail record");
      sd = "SD Error";
    }
    u8g2.setCursor(2, 62);
    u8g2.print(sd);
    //u8g2.sendBuffer(); 
    temp = "";
    hum = "";    
    u8g2.sendBuffer(); 
  }
  seg = seg + 1;
}
