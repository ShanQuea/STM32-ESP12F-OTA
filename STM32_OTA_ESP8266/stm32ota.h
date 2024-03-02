#ifndef _STM32OTA_H_
#define _STM32OTA_H_

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <SoftwareSerial.h>
#include <FS.h>

#define STERR         "错误"
#define STM32INIT     0x7F          // 发送初始化命令
#define STM32ACK      0x79          // 返回 ACK 应答
#define STM32NACK     0x1F          // 返回 NACK 应答
#define STM32GET      0             // 获取版本命令
#define STM32GVR      0x01          // 获取读保护状态          此处从未使用
#define STM32ID       0x02          // 获取芯片 ID 命令
#define STM32RUN      0x21          // 重新启动并运行程序
#define STM32RD       0x11          // 读取闪存命令            此处从未使用
#define STM32WR       0x31          // 写入闪存命令
#define STM32ERASE    0x43          // 擦除闪存命令
#define STM32ERASEN   0x44          // 擦除扩展闪存命令
#define STM32WP       0x63          // 写保护命令             此处从未使用
#define STM32WP_NS    0x64          // 无延伸写保护命令        此处从未使用
#define STM32UNPCTWR  0x73          // 未保护 WR 命令          此处从未使用
#define STM32UW_NS    0x74          // 无延伸写未保护命令       此处从未使用
#define STM32RP       0x82          // 读保护命令             此处从未使用
#define STM32RP_NS    0x83          // 无延伸读保护命令        此处从未使用
#define STM32UR       0x92          // 未保护读命令           此处从未使用
#define STM32UR_NS    0x93          // 无延伸未保护读命令      此处从未使用
#define STM32XOR      0xFF          // 未保护读命令            此处从未使用

#define STM32STADDR   0x8000000     // STM32 代码起始地址，如果使用自定义引导加载程序，可以更改为其他地址，如 0x8002000
#define STM32ERR      0

class stm32ota
{
  private:
    int pin_NRST;
    int pin_BOOT0;
    int pin_LED;

    void SendCommand(unsigned char commd); // 发送命令
    unsigned char GetId(); // 获取芯片ID
    unsigned char Erase(); // 擦除闪存
    unsigned char Erasen(); // 擦除扩展闪存
    unsigned char Address(unsigned long addr); // 设置地址
    unsigned char SendData(unsigned char * data, unsigned char wrlen); // 发送数据
    unsigned char getChecksum( unsigned char * data, unsigned char datalen); // 计算校验和

  public:
    stm32ota(int _NRST, int _BOOT0 , int _LED); // 构造函数
    String otaUpdate(String File_Url); // OTA更新
    void FlashMode(); // 设置闪存模式
    void RunMode(); // 设置运行模式
    String conect(); // 连接
    char chipVersion(); // 获取芯片版本
    String binfilename(); // 获取二进制文件名
    boolean  downloadFile(String File_Url , String File_Name); // 下载文件
    void deletfiles(String bin_file); // 删除文件
    boolean Flash(String bin_file_name); // 闪存
    boolean EraseChip(); // 擦除芯片
    String stm32Read(unsigned long rdaddress, unsigned char rdlen); // 读取闪存内容
};

#endif
