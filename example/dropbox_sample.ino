#include <Arduino.h>
#include "DropboxManager.h"
#include <ESP8266WiFi.h>

#define SSID			"YourSSID"
#define PASS			"YourPass"
#define TOKEN			"Dropbox_Token_here"

DropboxManager drop;
void setup()
{
	Serial.begin(115200);
	Serial.print("connecting to ");
	Serial.println(SSID);
	WiFi.mode(WIFI_STA);
	WiFi.begin(SSID, PASS);
	while (WiFi.status() != WL_CONNECTED)
	{
		delay(500);
		Serial.print(".");
	}
	Serial.println("");
	Serial.println("WiFi connected");
	Serial.println("IP address: ");
	Serial.println(WiFi.localIP());
	
	drop.SetToken(TOKEN);
	drop.fileUpload("/apple.txt","/spiffs.txt",1);
	// drop.fileDownload("/apple.txt","/fifrst.txt");
	// drop.fileDelete("/spiff's.txt");
}

void loop()
{
	// put your main code here, to run repeatedly:
}