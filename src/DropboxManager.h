#ifndef _DROPBOX_MANAGER_H_
#define _DROPBOX_MANAGER_H_
#include <Arduino.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include <FS.h>

#define DROPBOX_DEBUG_EN

#ifdef DROPBOX_DEBUG_EN
    #define DROPBOX_DEBUG(HEADER, ...)      Serial.print(HEADER);               \
                                            Serial.print(__LINE__);             \
                                            Serial.print(" : ");                \
                                            Serial.printf(__VA_ARGS__)
    #define DROPBOX_DEBUG_NL(HEADER, ...)   Serial.print(HEADER);               \
                                            Serial.print(__LINE__);             \
                                            Serial.print(" : ");                \
                                            Serial.printf(__VA_ARGS__);         \
                                            Serial.println();
#else
    #define DROPBOX_DEBUG(...)
    #define DROPBOX_DEBUG_NL(...)
#endif

#ifdef DROPBOX_DEBUG_EN
    #define DB_MSG(...)         DROPBOX_DEBUG("[DROPBOX] : ", __VA_ARGS__)
    #define DB_MSG_NL(...)      DROPBOX_DEBUG_NL("[DROPBOX] : ", __VA_ARGS__)
#else
    #define DB_MSG(...)
    #define DB_MSG_NL(...)
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