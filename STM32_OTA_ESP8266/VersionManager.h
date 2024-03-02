#ifndef VersionManager_h
#define VersionManager_h

#include <Arduino.h>

class VersionManager {
  public:
    VersionManager();
    void getVersion();
    int getNewVersion();
    void updateVersion(int version);
    int getVersionValue(); 
  private:
    int version;
    const char* date;
    const char* author;
    const char* version_url = "http://192.168.3.69:5000/version";
};



#endif
