/*
    STM32 MCU       NodeMCU引脚(基于ESP8266)
    PA9             RXD
    PA10            TXD
    BOOT0           D2
    RST             D1
    3.3V            3.3V
    GND             GND
    En -> 10K -> 3.3V
    TX-串行(PC)                             D6
    RX-串行(PC)                             D7
*/
#include "stm32ota.h"
#include "versionManager.h"
#include <ESP8266WiFi.h>
//#include <WiFiClient.h>
//#include <ESP8266WebServer.h>
//#include <ESP8266mDNS.h>
#include <FS.h>
#include <ESP8266HTTPClient.h>
#include <SoftwareSerial.h>
#include "ArduinoJson.h"  //版本6.15.2
#define ARDUINOJSON_USE_LONG_LONG 1

SoftwareSerial Debug;  //仅用于调试
#define MYPORT_TX 14   //仅用于调试
#define MYPORT_RX 13   //仅用于调试

stm32ota STM32(5, 4, 2);  //与STM32OTA库一起使用
VersionManager versi;


const char* ssid = "HONOR-F106A7";  //WIFI名
const char* password = "88889999";  //密码
const char* link_Updt = link_Updt = "http://192.168.3.69:5000/";
const char* version_url  = "http://192.168.3.69:5000/download/";


const char* date_url = "http://192.168.3.69:5000/version_date";
char link_bin[100];


unsigned long lastMinuteCheck = 0; // 用于跟踪上次一分钟检查的时间
const unsigned long minuteInterval = 60000; // 一分钟的毫秒数

boolean MandatoryUpdate = false;
//----------------------------------------------------------------------------------
const int buttonPin = 9;
const int ledPin = 2;
boolean aux = false;
unsigned long lastTime;
int button = true;

//----------------------------------------------------------------------------------
void wifiConnect() {
  Debug.println("");
  WiFi.disconnect(true);  
  WiFi.mode(WIFI_STA);
  delay(2000);  
  WiFi.begin(ssid, password);

  byte b = 0;
  while (WiFi.status() != WL_CONNECTED && b < 60) {  //连接尝试时间
    b++;
    Debug.print(".");
    delay(500);
  }
  Debug.println("");
  Debug.print("IP:");
  Debug.println(WiFi.localIP());
}
//----------------------------------------------------------------------------------
void checkupdt(boolean all = true) {
  if (WiFi.status() != WL_CONNECTED) {
    return;
  }
  WiFiClient client;
  HTTPClient http;
  http.begin(client, link_Updt);
  int httpCode = http.GET();
  String s = "";
  s = http.getString();
  http.end();
  s.trim();

  if (all) {
   // Serial.println(s);  //仅在调试中使用
  }


  if (httpCode != HTTP_CODE_OK) {
    return;
  }

  StaticJsonDocument<800> doc;
  deserializeJson(doc, s);
  strlcpy(link_bin, doc["link"] | "", sizeof(link_bin));
  MandatoryUpdate = doc["mandatory"] | false;
  Debug.println(link_bin);  //仅用于调试
  //Debug.println(MandatoryUpdate);                   //仅用于调试
}

//----------------------------------------------------------------------------------
void setup() {
  // Debug.begin(9600, SWSERIAL_8N1, MYPORT_RX, MYPORT_TX, false);  //仅用于调试
  Serial.begin(115200, SERIAL_8E1);
  Debug.println("DEBUG SOFTWARESERIAL");
  SPIFFS.begin();
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  delay(200);
  wifiConnect(); 
  delay(200);
  //STM32.RunMode();
  
  checkupdt();
  Debug.println("初始化结束");

  updatebin();
 
}

void updatebin()
{
    digitalWrite(ledPin, HIGH);
    Serial.println("开始更新");
    delay(2000);
    checkupdt(false);
    // String myString = String(link_bin);
    int versionDate;
    versionDate = versi.getNewVersion();
    if(versionDate) {
      String updateUrl = version_url + String(versionDate);
      Serial.println(STM32.otaUpdate(updateUrl));
      versi.updateVersion(versionDate);
    }
    
    Serial.println();
    Serial.println("更新结束");          
}



void loop() 
{
  unsigned long currentMillis = millis(); // 获取当前时间

  // 检查是否已经过了一分钟
  if (currentMillis - lastMinuteCheck >= minuteInterval) {            
  
   // Serial.println("One minute has passed!");
    versi.getVersion();
    // 更新上次一分钟检查的时间
    lastMinuteCheck = currentMillis;
  }


  if (currentMillis - lastTime > 500) {            
    if (aux) {
      aux = false;
    } else aux = true;
    lastTime = currentMillis;
    digitalWrite(ledPin, aux);
    // Serial.println(versi.getVersionValue());
  }
}
