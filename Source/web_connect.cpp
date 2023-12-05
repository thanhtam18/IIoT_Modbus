#include <web_connect.h>

boolean connect_client()
{
  boolean x;
  if (WiFi.status() != WL_CONNECTED)
  {
    digitalWrite(2, LOW);
    WiFi.begin(ssid.c_str(), pass.c_str());
    Serial.println("Try to connect WiFi");
    x = true;
  }
  if (global_ == false)
  {
    WiFiClient client;
    if (!client.connect(host, httpPort, 1000))
    {
      x = true;
      Serial.println("Connect to host failed");
      counter++;
      if (counter == 10)
      {
        ESP.restart();
      }
    }
    else
    {
      x = false; 
      counter=0;
    }
    client.stop();
    return x;
  }
}

String connect_client1(int givin1_, int givin2_, int givin3_, float givtem_, float givhum_)
{
  String getval_ = "";
  if (global_ == true)
  {
    HTTPClient http1;
    String url1 = "/sendstatus.php/";
    url1 += "?in1=" + String(givin1_) + "&in2=" + String(givin2_) + "&in3=" + String(givin3_) + "&tem=" + String(givtem_) + "&hum=" + String(givhum_);
    http1.begin("https://" + String(host) + url1, test_root_ca);
    int httpCode1 = http1.GET();
    delay(500);
    if (httpCode1 > 0)
    {
      if (httpCode1 == HTTP_CODE_OK)
      {
        getval_ = http1.getString();
      }
    }
    else
    {
      Serial.println("Connection1* Failed");
      if (autoconnect_flag == false)
      {
        task_number = 1;
        xTaskCreatePinnedToCore(reconnect, "reconnect", 6000, NULL, 0, &xReconnectHandle, 0);
        vTaskSuspend(xBoardHandle);
        vTaskSuspend(xStatusHandle);
      }
    }
    http1.end();
    return getval_;
  }
  else
  {
    WiFiClient client1;
    if (client1.connect(host, httpPort, 1000))
    {
      String url1 = "/sendstatus.php/";
      url1 += "?in1=" + String(givin1_) + "&in2=" + String(givin2_) + "&in3=" + String(givin3_) + "&tem=" + String(givtem_) + "&hum=" + String(givhum_);
      client1.print("GET https://" + String(host) + url1 + "HTTP/1.1\r\n" + "Host:" + String(host) + "\r\n" + "Connection: close\r\n\r\n");
    }
    else
    {
      client1.stop();
      Serial.println("Connection1 Failed");
      if (autoconnect_flag == false)
      {
        task_number = 1;
        xTaskCreatePinnedToCore(reconnect, "reconnect", 6000, NULL, 0, &xReconnectHandle, 0);
        vTaskSuspend(xBoardHandle);
        vTaskSuspend(xStatusHandle);
      }
    }
    delay(200);
    while (client1.connected())
    {
      getval_ = client1.readStringUntil('\r');
    }
    client1.stop();
    return getval_;
  }
}

String connnect_client2()
{
  String getbtn_ = "";
  if (global_ == true)
  {
    HTTPClient http2;
    String url2 = "/sendbtn.php/";
    http2.begin("https://" + String(host) + url2, test_root_ca);
    int httpCode2 = http2.GET();
    delay(500);
    if (httpCode2 > 0)
    {
      if (httpCode2 == HTTP_CODE_OK)
      {
        getbtn_ = http2.getString();
      }
    }
    else
    {
      Serial.println("Connection2* Failed");
      if (autoconnect_flag == false)
      {
        task_number = 2;
        xTaskCreatePinnedToCore(reconnect, "reconnect", 6000, NULL, 0, &xReconnectHandle, 0);
        vTaskSuspend(xPageHandle);
      }
    }
    http2.end();
    return getbtn_;
  }
  else
  {
    WiFiClient client2;
    if (client2.connect(host, httpPort, 1000))
    {
      String url2 = "/sendbtn.php/";
      client2.print("GET https://" + String(host) + url2 + "HTTP/1.1\r\n" + "Host:" + String(host) + "\r\n" + "Connection: close\r\n\r\n");
    }
    else
    {
      Serial.println("Connection2 Failed");
      client2.stop();
      if (autoconnect_flag == false)
      {
        task_number = 2;
        xTaskCreatePinnedToCore(reconnect, "reconnect", 6000, NULL, 0, &xReconnectHandle, 0);
        vTaskSuspend(xPageHandle);
      }
    }
    delay(200);
    while (client2.connected())
    {
      getbtn_ = client2.readStringUntil('\r');
    }
    client2.stop();
    return getbtn_;
  }
}

String connect_client3()
{
  String line_ = "";
  if (global_ == true)
  {
    HTTPClient http3;
    String url3 = "/senddata.php/";
    http3.begin("https://" + String(host) + url3, test_root_ca);
    int httpCode3 = http3.GET();
    delay(500);
    if (httpCode3 > 0)
    {
      if (httpCode3 == HTTP_CODE_OK)
      {
        line_ = http3.getString();
      }
    }
    else
    {
      Serial.println("Connection3* Failed");
      if (autoconnect_flag == false)
      {
        task_number = 3;
        xTaskCreatePinnedToCore(reconnect, "reconnect", 6000, NULL, 0, &xReconnectHandle, 0);
        vTaskSuspend(xGetdataHandle);
      }
    }
    http3.end();
    return line_;
  }
  else
  {
    WiFiClient client3;
    if (client3.connect(host, httpPort, 1000))
    {
      String url3 = "/senddata.php/";
      client3.print("GET https://" + String(host) + url3 + "HTTP/1.1\r\n" + "Host:" + String(host) + "\r\n" + "Connection: close\r\n\r\n");
    }
    else
    {
      Serial.println("Connection3 Failed");
      client3.stop();
      if (autoconnect_flag == false)
      {
        task_number = 3;
        xTaskCreatePinnedToCore(reconnect, "reconnect", 6000, NULL, 0, &xReconnectHandle, 0);
        vTaskSuspend(xGetdataHandle);
      }
    }
    delay(200);
    while (client3.connected())
    {
      line_ = client3.readStringUntil('\r');
    }
    client3.stop();
    return line_;
  }
}

String connect_client4(int giv_func_, int giv_address_, int giv_quantity_)
{
  String line_ = "";
  if (global_ == true)
  {
    HTTPClient http4;
    String url4 = "/getdata.php/";
    if (giv_func_ == 1 || giv_func_ == 3 || giv_func_ == 4 || giv_func_ == 2)
    {
      int k = 0;

      if (giv_address_ == 0)
      {
        k = giv_quantity_;
      }
      else
      {
        k = giv_quantity_ + giv_address_;
      }

      url4 += "?alias=";
      for (int i = giv_address_; i < k; i++)
      {
        url4 += String(i) + "-";
      }
      url4 += "&value=";
      if (giv_func_ == 3 || giv_func_ == 4)
      {
        for (int i = 0; i < giv_quantity_; i++)
        {
          url4 += String(au16data[i]) + "-";
        }
      }
      else
      {
        int j = 1;
        for (int i = 0; i < giv_quantity_; i++)
        {
          url4 += String(((au16data[0] & j) >> i)) + "-";
          j = j * 2;
        }
      }
    }
    else
    {
      if (giv_func_ == 5)
      {
        url4 += "?alias=" + String(giv_address_) + "-" + "&value=" + String((au16data[0] & 1)) + "-";
      }
      else
      {
        url4 += "?alias=" + String(giv_address_) + "-" + "&value=" + String(au16data[0]) + "-";
      }
    }
    http4.begin("https://" + String(host) + url4, test_root_ca);
    int httpCode4 = http4.GET();
    delay(500);
    if (httpCode4 > 0)
    {
      if (httpCode4 == HTTP_CODE_OK)
      {
        line_ = http4.getString();
      }
    }
    else
    {
      Serial.println("Connection4* Failed");
      if (autoconnect_flag == false)
      {
        task_number = 4;
        xTaskCreatePinnedToCore(reconnect, "reconnect", 6000, NULL, 0, &xReconnectHandle, 0);
        vTaskSuspend(xModbusHandle);
        vTaskSuspend(xSendHandle);
      }
    }
    http4.end();
    return line_;
  }
  else
  {
    WiFiClient client4;
    if (client4.connect(host, httpPort, 1000))
    {
      String url4 = "/getdata.php/";
      if (giv_func_ == 1 || giv_func_ == 3 || giv_func_ == 4 || giv_func_ == 2)
      {
        int k = 0;

        if (giv_address_ == 0)
        {
          k = giv_quantity_;
        }
        else
        {
          k = giv_quantity_ + giv_address_;
        }

        url4 += "?alias=";
        for (int i = giv_address_; i < k; i++)
        {
          url4 += String(i) + "-";
        }
        url4 += "&value=";
        if (giv_func_ == 3 || giv_func_ == 4)
        {
          for (int i = 0; i < giv_quantity_; i++)
          {
            url4 += String(au16data[i]) + "-";
          }
        }
        else
        {
          int j = 1;
          for (int i = 0; i < giv_quantity_; i++)
          {
            url4 += String(((au16data[0] & j) >> i)) + "-";
            j = j * 2;
          }
        }
      }
      else
      {
        if (giv_func_ == 5)
        {
          url4 += "?alias=" + String(giv_address_) + "-" + "&value=" + String((au16data[0] & 1)) + "-";
        }
        else
        {
          url4 += "?alias=" + String(giv_address_) + "-" + "&value=" + String(au16data[0]) + "-";
        }
      }
      client4.print("GET https://" + String(host) + url4 + "HTTP/1.1\r\n" + "Host:" + String(host) + "\r\n" + "Connection: close\r\n\r\n");
    }
    else
    {
      Serial.println("Connection4 Failed");
      client4.stop();
      if (autoconnect_flag == false)
      {
        task_number = 4;
        xTaskCreatePinnedToCore(reconnect, "reconnect", 6000, NULL, 0, &xReconnectHandle, 0);
        vTaskSuspend(xModbusHandle);
        vTaskSuspend(xSendHandle);
      }
    }
    delay(200);
    while (client4.connected())
    {
      line_ = client4.readStringUntil('\r');
    }
    client4.stop();
    return line_;
  }
}

String connect_client5(int c, int i, int get_data2_)
{
  String line_ = "";
  if (global_ == true)
  {
    HTTPClient http5;
    int httpCode5;
    if (c == 0)
    {
      String url5 = "/sendformsave.php/";
      http5.begin("https://" + String(host) + url5, test_root_ca);
    }
    else if (c == 1)
    {
      String url5 = "/getdatasave.php/?alias=0&val=0";
      url5 += "&v=" + String(datafr[0]) + "&a=" + String(datafr[1]) + "&hz=" + String(datafr[2]);
      http5.begin("https://" + String(host) + url5, test_root_ca);
    }
    else
    {
      String url5 = "/getdatasave.php/?v=0&a=0&hz=0";
      url5 += "&alias=" + String(get_data2_ + i) + "&val=" + String(dataplc[i]);
      http5.begin("https://" + String(host) + url5, test_root_ca);
    }
    httpCode5 = http5.GET();
    delay(500);
    if (httpCode5 > 0)
    {
      if (httpCode5 == HTTP_CODE_OK)
      {
        line_ = http5.getString();
      }
    }
    else
    {
      Serial.println("Connection5* Failed");
      if (autoconnect_flag == false)
      {
        task_number = 5;
        xTaskCreatePinnedToCore(reconnect, "reconnect", 6000, NULL, 0, &xReconnectHandle, 0);
        if (mbsavedata_flag == true)
        {
          vTaskSuspend(xMbsaveHandle);
        }
        if (timee_flag == true)
        {
          vTaskSuspend(xTimeHandle);
        }
        vTaskSuspend(xFilemanager);
      }
    }
    http5.end();
    return line_;
  }
  else
  {
    WiFiClient client5;
    if (c == 0)
    {
      if (client5.connect(host, httpPort, 1000))
      {
        String url5 = "/sendformsave.php/";
        client5.print("GET https://" + String(host) + url5 + "HTTP/1.1\r\n" + "Host:" + String(host) + "\r\n" + "Connection: close\r\n\r\n");
      }
      else
      {
        Serial.println("Connection5 Failed");
        client5.stop();
        if (autoconnect_flag == false)
        {
          task_number = 5;
          xTaskCreatePinnedToCore(reconnect, "reconnect", 6000, NULL, 0, &xReconnectHandle, 0);
          if (mbsavedata_flag == true)
          {
            vTaskSuspend(xMbsaveHandle);
          }
          if (timee_flag == true)
          {
            vTaskSuspend(xTimeHandle);
          }
          vTaskSuspend(xFilemanager);
        }
      }
    }
    else if (c == 1)
    {
      if (client5.connect(host, httpPort, 1000))
      {
        String url5 = "/getdatasave.php/?alias=0&val=0";
        url5 += "&v=" + String(datafr[0]) + "&a=" + String(datafr[1]) + "&hz=" + String(datafr[2]);
        client5.print("GET https://" + String(host) + url5 + "HTTP/1.1\r\n" + "Host:" + String(host) + "\r\n" + "Connection: close\r\n\r\n");
      }
      else
      {
        Serial.println("Connection5 Failed");
        client5.stop();
        if (autoconnect_flag == false)
        {
          task_number = 5;
          xTaskCreatePinnedToCore(reconnect, "reconnect", 6000, NULL, 0, &xReconnectHandle, 0);
          if (mbsavedata_flag == true)
          {
            vTaskSuspend(xMbsaveHandle);
          }
          if (timee_flag == true)
          {
            vTaskSuspend(xTimeHandle);
          }
          vTaskSuspend(xFilemanager);
        }
      }
    }
    else
    {
      if (client5.connect(host, httpPort, 1000))
      {
        String url5 = "/getdatasave.php/?v=0&a=0&hz=0";
        url5 += "&alias=" + String(get_data2_ + i) + "&val=" + String(dataplc[i]);
        client5.print("GET https://" + String(host) + url5 + "HTTP/1.1\r\n" + "Host:" + String(host) + "\r\n" + "Connection: close\r\n\r\n");
      }
      else
      {
        Serial.println("Connection5 Failed");
        client5.stop();
        if (autoconnect_flag == false)
        {
          task_number = 5;
          xTaskCreatePinnedToCore(reconnect, "reconnect", 6000, NULL, 0, &xReconnectHandle, 0);
          if (mbsavedata_flag == true)
          {
            vTaskSuspend(xMbsaveHandle);
          }
          if (timee_flag == true)
          {
            vTaskSuspend(xTimeHandle);
          }
          vTaskSuspend(xFilemanager);
        }
      }
    }
    delay(200);
    while (client5.connected())
    {
      line_ = client5.readStringUntil('\r');
    }
    client5.stop();
    return line_;
  }
}

void connect_client6()
{
  if (global_ == true)
  {
    HTTPClient http6;
    String url6 = "/stopsave.php/";
    http6.begin("https://" + String(host) + url6, test_root_ca);
    int httpCode6 = http6.GET();
    delay(500);
    if (httpCode6 > 0)
    {
      if (httpCode6 == HTTP_CODE_OK)
      {
        String t = http6.getString();
      }
    }
    else
    {
      Serial.println("Connection6* Failed");
      if (autoconnect_flag == false)
      {
        task_number = 6;
        xTaskCreatePinnedToCore(reconnect, "reconnect", 6000, NULL, 0, &xReconnectHandle, 0);
        vTaskSuspend(xTimeHandle);
      }
    }
    http6.end();
  }
  else
  {
    WiFiClient client6;
    if (client6.connect(host, httpPort, 1000))
    {
      String url6 = "/stopsave.php/";
      client6.print("GET https://" + String(host) + url6 + "HTTP/1.1\r\n" + "Host:" + String(host) + "\r\n" + "Connection: close\r\n\r\n");
    }
    else
    {
      Serial.println("Connection6 Failed");
      client6.stop();
      if (autoconnect_flag == false)
      {
        task_number = 6;
        xTaskCreatePinnedToCore(reconnect, "reconnect", 6000, NULL, 0, &xReconnectHandle, 0);
        vTaskSuspend(xTimeHandle);
      }
    }
    delay(200);
    client6.stop();
  }
}

String connect_client7()
{
  String line_ = "";
  if (global_ == true)
  {
    HTTPClient http7;
    String url7 = "/sendfr.php/";
    http7.begin("https://" + String(host) + url7, test_root_ca);
    int httpCode7 = http7.GET();
    delay(500);
    if (httpCode7 > 0)
    {
      if (httpCode7 == HTTP_CODE_OK)
      {
        line_ = http7.getString();
      }
    }
    else
    {
      Serial.println("Connection7* Failed");
      if (autoconnect_flag == false)
      {
        task_number = 7;
        xTaskCreatePinnedToCore(reconnect, "reconnect", 6000, NULL, 0, &xReconnectHandle, 0);
        vTaskSuspend(xGetdatafrHandle);
      }
    }
    http7.end();
    return line_;
  }
  else
  {
    WiFiClient client7;
    if (client7.connect(host, httpPort, 1000))
    {
      String url7 = "/sendfr.php/";
      client7.print("GET https://" + String(host) + url7 + "HTTP/1.1\r\n" + "Host:" + String(host) + "\r\n" + "Connection: close\r\n\r\n");
    }
    else
    {
      Serial.println("Connection3 Failed");
      client7.stop();
      if (autoconnect_flag == false)
      {
        task_number = 7;
        xTaskCreatePinnedToCore(reconnect, "reconnect", 6000, NULL, 0, &xReconnectHandle, 0);
        vTaskSuspend(xGetdatafrHandle);
      }
    }
    delay(200);
    while (client7.connected())
    {
      line_ = client7.readStringUntil('\r');
    }
    client7.stop();
    return line_;
  }
}

String connect_client8(int c)
{
  String line_ = "";
  if (global_ == true)
  {
    HTTPClient http8;
    int httpCode8;
    if (c == 0)
    {
      String url8 = "/getdatafr.php/";
      url8 += "?v=" + String(au16datafr[0]) + "&a=" + String(au16datafr[1]) + "&hz=" + String(au16datafr[2]) + "&r=" + String(au16datafr[3]);
      http8.begin("https://" + String(host) + url8, test_root_ca);
    }
    else if (c == 1)
    {
      String url8 = "/senddatafr.php/";
      http8.begin("https://" + String(host) + url8, test_root_ca);
    }
    else
    {
      String url8 = "/refeshfr.php/";
      http8.begin("https://" + String(host) + url8, test_root_ca);
    }
    httpCode8 = http8.GET();
    delay(500);
    if (httpCode8 > 0)
    {
      if (httpCode8 == HTTP_CODE_OK)
      {
        line_ = http8.getString();
      }
    }
    else
    {
      Serial.println("Connection8* Failed");
      if (autoconnect_flag == false)
      {
        task_number = 8;
        xTaskCreatePinnedToCore(reconnect, "reconnect", 6000, NULL, 0, &xReconnectHandle, 0);
        vTaskSuspend(xMbfrHandle);
        vTaskSuspend(xSenddatafrHandle);
      }
    }
    http8.end();
    return line_;
  }
  else
  {
    WiFiClient client8;
    if (c == 0)
    {
      if (client8.connect(host, httpPort, 1000))
      {
        String url8 = "/getdatafr.php/";
        url8 += "?v=" + String(au16datafr[0]) + "&a=" + String(au16datafr[1]) + "&hz=" + String(au16datafr[2]) + "&r=" + String(au16datafr[3]);
        client8.print("GET https://" + String(host) + url8 + "HTTP/1.1\r\n" + "Host:" + String(host) + "\r\n" + "Connection: close\r\n\r\n");
      }
      else
      {
        Serial.println("Connection8 Failed");
        client8.stop();
        if (autoconnect_flag == false)
        {
          task_number = 8;
          xTaskCreatePinnedToCore(reconnect, "reconnect", 6000, NULL, 0, &xReconnectHandle, 0);
          vTaskSuspend(xMbfrHandle);
          vTaskSuspend(xSenddatafrHandle);
        }
      }
    }
    else if (c == 1)
    {
      if (client8.connect(host, httpPort, 1000))
      {
        String url8 = "/senddatafr.php/";
        client8.print("GET https://" + String(host) + url8 + "HTTP/1.1\r\n" + "Host:" + String(host) + "\r\n" + "Connection: close\r\n\r\n");
      }
      else
      {
        Serial.println("Connection8 Failed");
        client8.stop();
        if (autoconnect_flag == false)
        {
          task_number = 8;
          xTaskCreatePinnedToCore(reconnect, "reconnect", 6000, NULL, 0, &xReconnectHandle, 0);
          vTaskSuspend(xMbfrHandle);
          vTaskSuspend(xSenddatafrHandle);
        }
      }
    }
    else
    {
      if (client8.connect(host, httpPort, 1000))
      {
        String url8 = "/refeshfr.php/";
        client8.print("GET https://" + String(host) + url8 + "HTTP/1.1\r\n" + "Host:" + String(host) + "\r\n" + "Connection: close\r\n\r\n");
      }
      else
      {
        Serial.println("Connection8 Failed");
        client8.stop();
        if (autoconnect_flag == false)
        {
          task_number = 8;
          xTaskCreatePinnedToCore(reconnect, "reconnect", 6000, NULL, 0, &xReconnectHandle, 0);
          vTaskSuspend(xMbfrHandle);
          vTaskSuspend(xSenddatafrHandle);
        }
      }
    }
    delay(200);
    while (client8.connected())
    {
      line_ = client8.readStringUntil('\r');
    }
    client8.stop();
    return line_;
  }
}

String connect_client9()
{
  String line_ = "";
  if (global_ == true)
  {
    HTTPClient http9;
    String url9 = "/sendsetting.php/";
    http9.begin("https://" + String(host) + url9, test_root_ca);
    int httpCode9 = http9.GET();
    delay(500);
    if (httpCode9 > 0)
    {
      if (httpCode9 == HTTP_CODE_OK)
      {
        line_ = http9.getString();
      }
    }
    else
    {
      Serial.println("Connection9* Failed");
      if (autoconnect_flag == false)
      {
        task_number = 9;
        xTaskCreatePinnedToCore(reconnect, "reconnect", 6000, NULL, 0, &xReconnectHandle, 0);
        vTaskSuspend(xSettingHandle);
      }
    }
    http9.end();
    return line_;
  }
  else
  {
    WiFiClient client9;
    if (client9.connect(host, httpPort, 1000))
    {
      String url9 = "/sendsetting.php/";
      client9.print("GET https://" + String(host) + url9 + "HTTP/1.1\r\n" + "Host:" + String(host) + "\r\n" + "Connection: close\r\n\r\n");
    }
    else
    {
      Serial.println("Connection9 Failed");
      client9.stop();
      if (autoconnect_flag == false)
      {
        task_number = 9;
        xTaskCreatePinnedToCore(reconnect, "reconnect", 6000, NULL, 0, &xReconnectHandle, 0);
        vTaskSuspend(xSettingHandle);
      }
    }
    delay(200);
    while (client9.connected())
    {
      line_ = client9.readStringUntil('\r');
    }
    client9.stop();
    return line_;
  }
}