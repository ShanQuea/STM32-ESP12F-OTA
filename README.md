# STM32-ESP12F-OTA

    STM32 MCU       NodeMCU引脚(基于ESP8266)
    PA9             RXD
    PA10            TXD
    BOOT0           D2
    RST             D1
    3.3V            3.3V
    GND             GND
通过python网页进行更新固件 当检测到更新时nodemcu会自动下载固件并更新到STM MCU中
目前只写了http的链接
