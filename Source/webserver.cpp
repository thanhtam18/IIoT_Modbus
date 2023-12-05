#include <webserver.h>

int change_string_to_int(String s)
{
  int x = 0;
  int l = s.length();
  for (int i = 0; i < l; i++)
  {
    x = x * 10 + (s[i] - 48);
  }
  return x;
}

void setupMode()
{
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  int n = WiFi.scanNetworks();
  delay(100);
  for (int i = 0; i < n; ++i)
  {
    ssidList += "<option value=\"";
    ssidList += WiFi.SSID(i);
    ssidList += "\">";
    ssidList += WiFi.SSID(i);
    ssidList += "</option>";
  }
  delay(100);
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(apSSID, "12345678");
  dnsServer.start(53, "*", apIP);
  startWebServer();
  Serial.print("Starting Access Point at \"");
  Serial.print(apSSID);
  Serial.println("\"");
}

String makePage(String title, String contents)
{
  String s = "<!DOCTYPE html><html><head>";
  s += "<meta name=\"viewport\" content=\"width=device-width,user-scalable=0\">";
  s += "<title>";
  s += title;
  s += "</title></head><body>";
  s += contents;
  s += "</body></html>";
  return s;
}

String urlDecode(String input)
{
  String s = input;
  s.replace("%20", " ");
  s.replace("+", " ");
  s.replace("%21", "!");
  s.replace("%22", "\"");
  s.replace("%23", "#");
  s.replace("%24", "$");
  s.replace("%25", "%");
  s.replace("%26", "&");
  s.replace("%27", "\'");
  s.replace("%28", "(");
  s.replace("%29", ")");
  s.replace("%30", "*");
  s.replace("%31", "+");
  s.replace("%2C", ",");
  s.replace("%2E", ".");
  s.replace("%2F", "/");
  s.replace("%2C", ",");
  s.replace("%3A", ":");
  s.replace("%3A", ";");
  s.replace("%3C", "<");
  s.replace("%3D", "=");
  s.replace("%3E", ">");
  s.replace("%3F", "?");
  s.replace("%40", "@");
  s.replace("%5B", "[");
  s.replace("%5C", "\\");
  s.replace("%5D", "]");
  s.replace("%5E", "^");
  s.replace("%5F", "-");
  s.replace("%60", "`");
  return s;
}

void startWebServer()
{
  Serial.print("Starting Web Server at ");
  Serial.println(WiFi.softAPIP());
  webServer.on("/settings", []()
               {
      String s = "<h1 style=\"text-align: center;\">Setting WiFi For ESP32</h1><p style=\"text-align: center;\">Please enter your password by selecting the SSID.</p>";
             s += "<form method=\"get\" action=\"setap\" style=\"text-align: center;\"><label>SSID: </label><select name=\"ssid\" style=\"width: 170px; margin-left: 28px;\">";
             s += ssidList;
             s += "</select><br>Password: <input name=\"pass\" length=20 type=\"password\"><br>Host:<input name=\"hostt\" length=30 type=\"text\" style=\"margin-left: 33px;\">";
             s += "<br>Port:<input name=\"port\" length=5 type=\"number\" style=\"margin-left: 33px;\"><br><input type=\"submit\"></form>";

      webServer.send(200, "text/html", makePage("Setting WiFi", s)); });
  webServer.on("/setap", []()
               {
      for (int i = 0; i < 70; ++i) {
        EEPROM.write(i, 0);               
      }
      String ssid = urlDecode(webServer.arg("ssid"));
      Serial.print("SSID: ");
      Serial.println(ssid);
      String pass = urlDecode(webServer.arg("pass"));
      Serial.print("Password: ");
      Serial.println(pass);
      String gethost = urlDecode(webServer.arg("hostt"));;
      Serial.print("Host: ");
      Serial.println(gethost);
      String getport = urlDecode(webServer.arg("port"));;
      Serial.print("Port: ");
      Serial.println(getport);
      Serial.println("Writing SSID to EEPROM...");
      for (int i = 0; i < ssid.length(); ++i) {
        EEPROM.write(i, ssid[i]);
      }
      Serial.println("Writing Password to EEPROM...");
      for (int i = 0; i < pass.length(); ++i) {
        EEPROM.write(20 + i, pass[i]);
      }
      for (int i = 0; i < gethost.length(); ++i) {
        EEPROM.write(40 + i, gethost[i]);
      }
      for (int i = 0; i < getport.length(); ++i) {
        EEPROM.write(70 + i, getport[i]);
      }
      int j= getport.length();
        EEPROM.write(75, j);
      EEPROM.commit();
      Serial.println("Write EEPROM done!");
      String s = "<h1>Setup complete.</h1><p>device will be connected to \"";
      s += ssid;
      s += "\" after the restart.";
      webServer.send(200, "text/html", makePage("Wi-Fi Settings", s));
      ESP.restart(); });
  webServer.onNotFound([]()
                       {
      String s = "<h1>AP mode</h1><p><a href=\"/settings\">Click here to config WiFi</a></p>";
      webServer.send(200, "text/html", makePage("AP mode", s)); });
  webServer.begin();
}

void deleteWiFi()
{
  int a = 0;
  while (a < 11)
  {
    digitalWrite(2, !digitalRead(2));
    a++;
    delay(200);
  }
  for (int i = 0; i < 70; ++i)
  {
    EEPROM.write(i, 0);
  }
  EEPROM.commit();
  Serial.println("WiFi is going to reset");
  ESP.restart();
}

boolean restoreConfig()
{
  Serial.println("Reading EEPROM...");
  if (EEPROM.read(0) != 0 || EEPROM.read(1) != 0 || EEPROM.read(2) != 0 || EEPROM.read(3) != 0)
  {
    for (int i = 0; i < 20; ++i)
    {
      ssid += char(EEPROM.read(i));
    }
    Serial.print("SSID: ");
    Serial.println(ssid);
    for (int i = 20; i < 40; ++i)
    {
      pass += char(EEPROM.read(i));
    }
    Serial.print("Password: ");
    Serial.println(pass);
    for (int i = 40; i < 70; ++i)
    {
      myhost += char(EEPROM.read(i));
      if (EEPROM.read(i) == 'w' && EEPROM.read(i + 1) == 'w' && EEPROM.read(i + 2) == 'w')
      {
        global_ = true;
      }
    }
    host = myhost.c_str();
    Serial.print("Host: ");
    Serial.println(host);
    int j;
    j = EEPROM.read(75);
    for (int i = 70; i < 70 + j; ++i)
    {
      myport += char(EEPROM.read(i));
    }
    httpPort = myport.toInt();
    Serial.print("Port: ");
    Serial.println(httpPort);
    WiFi.begin(ssid.c_str(), pass.c_str());
    delay(3000);
    return true;
  }
  else
  {
    Serial.println("Config not found.");
    return false;
  }
}