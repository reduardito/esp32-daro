#include <WiFi.h>
#include "time.h"
#include "DHT.h"
#include <U8g2lib.h>
#include <Wire.h>
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

String temp = "";
String hum = "";
int seg = 0;
String ip;
const char* ssid       = "clarowifi212";
const char* password   = "11223344";
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = -10800; // utc -3 bsas
const int   daylightOffset_sec = 0; // para horario de verano 3600

void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  //Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  Serial.print(F("NTP "));
  Serial.print(F(ntpServer));
  Serial.print(F(" "));
  Serial.println(&timeinfo, " %b-%d-%Y %H:%M:%S");
  Serial.println(F(""));
}

void defauloled()
{
  
}
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
  IPAddress myIP = WiFi.localIP();
  Serial.print("IP: ");
  Serial.println(myIP);
  ip = String() + WiFi.localIP()[0] + "." + WiFi.localIP()[1] + "." + WiFi.localIP()[2] + "." + WiFi.localIP()[3];
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  u8g2.begin(); //Inicializamos el dispositivo
  dht.begin();
}

void loop()
{
  delay(100);
  if (seg == 5 or seg == 0){
    seg = 0;
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    float hic = dht.computeHeatIndex(t, h, false);  Serial.print(F("Humedad: "));
    temp.concat(t);
    hum.concat(h);
    u8g2.clearBuffer();
//    u8g2.setFont(u8g2_font_ncenB08_tr);   
    u8g2.setFont(u8g2_font_ncenB14_tr);
    u8g2.setCursor(2, 15);
    u8g2.print("C    " + temp);
    u8g2.setCursor(2, 33);
    u8g2.print("%    " + hum);
    u8g2.setCursor(108, 15);
    u8g2.print("M");
    u8g2.setFont(u8g2_font_ncenB08_tr);
    u8g2.setCursor(2, 50);
    u8g2.print(" ip:  " + ip);
    u8g2.setCursor(2, 61);
    u8g2.print("mc:  " + WiFi.macAddress());
    u8g2.sendBuffer();                   
    temp = "";
    hum = "";    
    if (isnan(h) || isnan(t)) {
      Serial.println(F("Fallo lectura sensor DHT11!"));
      return;
    }
    Serial.print(h);
    Serial.print(F("%  Temperatura: "));
    Serial.print(t);
    Serial.print(F("°C | NTP: "));
    printLocalTime();
  }
  seg = seg + 1;
}
