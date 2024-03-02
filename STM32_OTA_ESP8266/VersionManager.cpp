#include "VersionManager.h"
#include <FS.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

VersionManager::VersionManager() {}

void VersionManager::getVersion() {
  Serial.println("尝试获取版本信息");
  File file = SPIFFS.open("/version.txt", "r");
  if (file) {
    String filedate = "";
    while (file.available()) {
      char c = file.read();
      filedate += c;
    }

    StaticJsonDocument<1024> doc;
    DeserializationError error = deserializeJson(doc, filedate);
    if (error) {
      Serial.print(F("JSON 解析错误: "));
      Serial.println(error.c_str());
      return;
    }

    version = doc["version"];
    date = doc["date"];
    author = doc["author"];

    Serial.print("当前版本号:");
    Serial.println(version);
    // Serial.print("日期:");
    // Serial.println(date);
    // Serial.print("作者:");
    // Serial.println(author);
    file.close();
  } else {
    Serial.println("打开版本文件失败");
  }
}



int VersionManager::getNewVersion() {
  HTTPClient httpClient;
  WiFiClient wifiClient;

  httpClient.begin(wifiClient, version_url);

  int httpCode = httpClient.GET();
  //Serial.print(httpCode);
  if (httpCode == HTTP_CODE_OK) {
    // Serial.println("测试成功");
    String payload = httpClient.getString();
    int remoteVersion = payload.toInt();
    httpClient.end();
    if (remoteVersion > version) {
      Serial.println("需要更新");
      return remoteVersion;
    }else {
      return 0;
    }
  }
  return 0;
}


void VersionManager::updateVersion(int version) {
  Serial.println("尝试更新版本信息");

  File file = SPIFFS.open("/version.txt", "r");
  if (!file) {
    Serial.println("版本文件不存在，创建新文件");
    file = SPIFFS.open("/version.txt", "w");
    if (!file) {
      Serial.println("无法创建版本文件");
      return;
    }
    StaticJsonDocument<1024> doc;
    String jsonString;
    serializeJson(doc, jsonString);
    file.print(jsonString);
    file.close();
    Serial.println("新版本文件已创建");
    return;
  }

  String filedate = "";
  while (file.available()) {
    char c = file.read();
    filedate += c;
  }
  file.close();

  if (filedate.length() == 0) {
    Serial.println("版本文件为空，直接写入新版本信息");
    file = SPIFFS.open("/version.txt", "w");
    if (!file) {
      Serial.println("无法打开版本文件");
      return;
    }
    StaticJsonDocument<1024> doc;
    doc["version"] = "1.0";
    doc["date"] = "2024-02-21";
    doc["author"] = "ShanQue";
    String jsonString;
    serializeJson(doc, jsonString);
    file.print(jsonString);
    file.close();
    Serial.println("版本信息更新成功");
    return;
  }

  StaticJsonDocument<1024> doc;
  DeserializationError error = deserializeJson(doc, filedate);
  if (error) {
    Serial.print(F("JSON 解析错误: "));
    Serial.println(error.c_str());
    return;
  }

  doc["version"] = String(version);
  Serial.print("更新后的版本号");
  Serial.println(version);
  doc["date"] = "2024-02-21";
  doc["author"] = "ShanQue";

  file = SPIFFS.open("/version.txt", "w");
  if (!file) {
    Serial.println("无法打开版本文件");
    return;
  }
  String jsonString;
  serializeJson(doc, jsonString);
  file.print(jsonString);
  file.close();
  Serial.println("版本信息更新成功");
}



int VersionManager::getVersionValue() {
    return version;
}