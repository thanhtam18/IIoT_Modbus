#ifndef __WEBSEVER_H
#define __WEBSEVER_H
#include <ESP32WebServer.h>
#include <DNSServer.h>
#include <EEPROM.h>
#include <WiFi.h>
const IPAddress apIP(192, 168, 1, 1);
const char *apSSID = "CONFIG WIFI ESP32";
String ssidList;
DNSServer dnsServer;
ESP32WebServer webServer(80);

extern String myhost;
extern String myport;
extern String ssid;
extern String pass;
extern boolean autoconnect_flag, global_, timee_flag, mbsave_flag;
extern const char *host;
extern int httpPort;

void setupMode();
String makePage(String title, String contents);
String urlDecode(String input);
void startWebServer();
boolean restoreConfig();
void deleteWiFi();

#endif