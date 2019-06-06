#include "DropboxManager.h"

#define DROPBOX_MAIN_API 					"https://api.dropboxapi.com"
#define DROPBOX_CONTENT_API 				"https://content.dropboxapi.com"
#define DROPBOX_CONTENT_API_FINGERPRINT 	"9D 86 7B C9 7E 07 D7 5C 86 66 A3 E2 95 C3 B5 45 C5 1E 89 B3"
#define DROPBOX_MAIN_API_FINGERPRINT 		"BE 7B 04 C2 5E C2 5A 2C 7E 9D 0B 1C E7 ED 89 AA E5 C2 4F E7"
#define DROPBOX_FILE_UPLOAD 				"/2/files/upload"
#define DROPBOX_FILE_DOWNLOAD 				"/2/files/download"
#define DROPBOX_FILE_DELETE 				"/2/files/delete"
#define AUTH 								"Authorization"
#define CONTENT_TYPE 						"Content-Type"
#define CONTENT_LEN 						"Content-Length"
#define TYPE_OCTET_STREAM 					"application/octet-stream"
#define TYPE_JSON 							"application/json"
#define DROPBOX_API_ARG 					"Dropbox-API-Arg"

DropboxManager::DropboxManager()
{
}

DropboxManager::~DropboxManager()
{
}

void DropboxManager::SetToken(String token)
{
	DB_MSG_NL("Setting Dropbox access token");
	_token = token;
}

bool DropboxManager::stringUpload(String str, String address, bool type)
{
	int httpCode = 0;
	String _mode = "";
	String _auth = "";
	String api_arg = "";
	String response = "";
	uint32_t len = 0;
	if (type)
	{
		_mode = "overwrite";
	}
	else
	{
		_mode = "add";
	}
	if (_token != '\0')
	{
		_auth = "Bearer " + _token;
		String DROP_URL = String(DROPBOX_CONTENT_API) + String(DROPBOX_FILE_UPLOAD);
		len = str.length();
		StaticJsonDocument<512> Arg;
		Arg["path"] = address;
		Arg["mode"] = _mode;
		Arg["autorename"] = true;
		Arg["mute"] = false;
		Arg["strict_conflict"] = false;
		serializeJson(Arg, api_arg);

		_wifi.setFingerprint(DROPBOX_CONTENT_API_FINGERPRINT);
		_http.begin(_wifi, DROP_URL);
		_http.addHeader(AUTH, _auth);
		_http.addHeader(CONTENT_TYPE, TYPE_OCTET_STREAM);
		_http.addHeader(CONTENT_LEN, String(len));
		_http.addHeader(DROPBOX_API_ARG, api_arg);
		httpCode = _http.POST(str);

		if (httpCode == HTTP_CODE_OK)
		{
			DB_MSG_NL("file Created");
			_http.end();
			return true;
		}
		DB_MSG_NL("error ;)");
		DB_MSG_NL("httpCode : %d", httpCode);
		response = _http.getString();
		DB_MSG_NL("%s", response.c_str());
		_http.end();
		return false;
	}
	DB_MSG_NL("Please Make Sure You have set valid token");
	return false;
}

bool DropboxManager::fileUpload(String file, String address, bool type)
{
	int httpCode = 0;
	String _mode = "";
	String _auth = "";
	String api_arg = "";
	String response = "";
	if (type)
	{
		_mode = "overwrite";
	}
	else
	{
		_mode = "add";
	}
	if (_token != '\0')
	{
		if (!SPIFFS.begin())
		{
			DB_MSG_NL("Failed To Mount FS");
			return false;
		}
		File fileUpload = SPIFFS.open(file, "r");
		if (!fileUpload)
		{
			DB_MSG_NL("No Such file Found to Upload;)");
			return false;
		}
		uint32_t file_size = fileUpload.size();
		if (file_size > 0)
		{
			DB_MSG_NL("File %s size is : %lu", file.c_str(), file_size);
			DB_MSG_NL("%ld", file_size);
			_auth = "Bearer " + _token;
			String DROP_URL = String(DROPBOX_CONTENT_API) + String(DROPBOX_FILE_UPLOAD);
			StaticJsonDocument<512> Arg;
			Arg["path"] = address;
			Arg["mode"] = _mode;
			Arg["autorename"] = true;
			Arg["mute"] = false;
			Arg["strict_conflict"] = false;
			serializeJson(Arg, api_arg);

			_wifi.setFingerprint(DROPBOX_CONTENT_API_FINGERPRINT);
			_http.begin(_wifi, DROP_URL);
			_http.addHeader(AUTH, _auth);
			_http.addHeader(CONTENT_TYPE, TYPE_OCTET_STREAM);
			_http.addHeader(CONTENT_LEN, String(file_size));
			_http.addHeader(DROPBOX_API_ARG, api_arg);
			httpCode = _http.sendRequest("POST", &fileUpload, file_size);
			response = _http.getString();
			fileUpload.close();
			SPIFFS.end();
			if (httpCode == HTTP_CODE_OK)
			{
				DB_MSG_NL("File uploaded/Created...");
				return true;
			}
			DB_MSG_NL("code : %d", httpCode);
			DB_MSG_NL("response : %s", response.c_str());
			return false;
		}
		DB_MSG_NL("File %s not found or valid ;)");
		fileUpload.close();
		SPIFFS.end();
	}
	return false;
}

bool DropboxManager::fileDownload(String file, String address)
{
	int httpCode = 0;
	String _mode = "";
	String _auth = "";
	String api_arg = "";
	String response = "";
	if (_token != '\0')
	{
		_auth = "Bearer " + _token;
		String DROP_URL = String(DROPBOX_CONTENT_API) + String(DROPBOX_FILE_DOWNLOAD);
		StaticJsonDocument<128> Arg;
		Arg["path"] = address;
		serializeJson(Arg, api_arg);

		_wifi.setFingerprint(DROPBOX_CONTENT_API_FINGERPRINT);
		_http.begin(_wifi, DROP_URL);
		_http.addHeader(AUTH, _auth);
		_http.addHeader(DROPBOX_API_ARG, api_arg);
		httpCode = _http.POST("");

		if (httpCode == HTTP_CODE_OK)
		{
			if (!SPIFFS.begin())
			{
				DB_MSG_NL("Failed To Mount FS");
				_http.end();
				SPIFFS.end();
				return false;
			}
			DB_MSG_NL("Downloading File To Spiffs using stream...");
			File fileDownload = SPIFFS.open(file, "w");
			if (fileDownload)
			{
				_http.writeToStream(&fileDownload);
				DB_MSG_NL("File Downloaded:)");
			}
			else
			{
				DB_MSG_NL("can't Access Spiffs ");
			}
			fileDownload.close();
			_http.end();
			SPIFFS.end();
			return true;
		}
		_http.end();
		DB_MSG_NL("code : %d", httpCode);
		DB_MSG_NL("Failed To Download File ;)");
		return false;
	}
}

bool DropboxManager::fileDelete(String file)
{
	int httpCode = 0;
	String _mode = "";
	String _auth = "";
	String jsonStr = "";
	String response = "";
	String DROP_URL = String(DROPBOX_MAIN_API) + String(DROPBOX_FILE_DELETE);
	_auth = "Bearer " + _token;
	StaticJsonDocument<128> json;
	json["path"] = file;
	serializeJson(json, jsonStr);

	_wifi.setFingerprint(DROPBOX_MAIN_API_FINGERPRINT);
	_http.begin(_wifi, DROP_URL);
	_http.addHeader(AUTH, _auth);
	_http.addHeader(CONTENT_TYPE, TYPE_JSON);
	httpCode = _http.POST(jsonStr);
	response = _http.getString();
	_http.end();
	if (httpCode == HTTP_CODE_OK)
	{
		DB_MSG_NL("File Deleted");
		return false;
	}
	DB_MSG_NL("Code : %d", httpCode);
	DB_MSG_NL("response : %s", response.c_str());
	return true;
}