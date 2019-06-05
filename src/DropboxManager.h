#ifndef _DROPBOX_MANAGER_H_
#define _DROPBOX_MANAGER_H_
#include <Arduino.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include <FS.h>


#define DROPBOX_DEBUG

#ifdef DROPBOX_DEBUG
    #define DEBUG_BAUDRATE          (115200)
    #define DEBUG_INIT()            Serial.begin(DEBUG_BAUDRATE)
    #define DEBUG(HEADER, ...)      Serial.print(HEADER);               \
                                    Serial.print(__LINE__);             \
                                    Serial.print(" : ");                \
                                    Serial.printf(__VA_ARGS__)
    #define DEBUG_NL(HEADER, ...)   Serial.print(HEADER);               \
                                    Serial.print(__LINE__);             \
                                    Serial.print(" : ");                \
                                    Serial.printf(__VA_ARGS__);         \
                                    Serial.println();
#else
    #define DEBUG_INIT()
    #define DEBUG_PRINT(x)
    #define DEBUG_PRINT(x)
    #define DEBUG(...)
    #define DEBUG_NL(...)
#endif

#ifdef DROPBOX_DEBUG
    #define DMSG(...)         DEBUG("[DROPBOX] : ", __VA_ARGS__)
    #define DMSG_NL(...)      DEBUG_NL("[DROPBOX] : ", __VA_ARGS__)
#else
    #define DMSG(...)
    #define DMSG_NL(...)
#endif

class DropboxManager
{
private:
    String              _token;
    WiFiClientSecure    _wifi;
    HTTPClient          _http;
public:
    DropboxManager();
    void SetToken(String token);
    bool fileUpload(String file, String address, bool type);
    bool fileDownload(String file, String address);
    bool fileDelete(String file);
    bool stringUpload(String str, String address, bool type);
    ~DropboxManager();
};

#endif  /* _DROPBOX_MANAGER_H_ */