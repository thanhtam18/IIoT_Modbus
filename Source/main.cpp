#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

#include <Arduino.h>
#include "main.h"
#include "webserver.h"
#include "web_connect.h"
#include "dht11.h"

#define DHT11PIN 32

void setup()
{
  Serial.begin(9600);
  master.begin(9600);
  EEPROM.begin(512);
  pinMode(13, INPUT);
  pinMode(2, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(18, OUTPUT);
  pinMode(4, OUTPUT);
  digitalWrite(18, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);
  master.setTimeOut(10000);
  delay(1000);
  digitalWrite(2, LOW);
  if (digitalRead(13) == 0)
  {
    deleteWiFi();
  }
  if (restoreConfig())
  {
    Serial.print("Waiting for Wi-Fi connection");
    while (WiFi.status() != WL_CONNECTED)
    {
      WiFi.begin(ssid.c_str(), pass.c_str());
      digitalWrite(2, !digitalRead(2));
      if (digitalRead(13) == 0)
      {
        deleteWiFi();
      }
      delay(500);
    }
    Serial.println();
    Serial.println("Connected!");
  }
  else
  {
    Serial.println("Please config WiFi");
    setupMode();
    while (WiFi.status() != WL_CONNECTED)
    {
      dnsServer.processNextRequest();
      webServer.handleClient();
    }
  }
  if (global_ == true)
  {
    Serial.println("Global connect");
    while (WiFi.status() != WL_CONNECTED)
    {
      WiFi.begin(ssid.c_str(), pass.c_str());
      if (digitalRead(13) == 0)
      {
        deleteWiFi();
      }
      digitalWrite(2, !digitalRead(2));
    }
  }
  else
  {
    Serial.println("Local connect");
    WiFiClient client;
    while (!client.connect(host, httpPort, 1000))
    {
      if (digitalRead(13) == 0)
      {
        deleteWiFi();
      }
      digitalWrite(2, !digitalRead(2));
    }
  }
  digitalWrite(2, HIGH);
  xQueuein1 = xQueueCreate(1, sizeof(int));
  xQueuein2 = xQueueCreate(1, sizeof(int));
  xQueuein3 = xQueueCreate(1, sizeof(int));
  xQueueout1 = xQueueCreate(1, sizeof(int));
  xQueueout2 = xQueueCreate(1, sizeof(int));
  xQueuetem = xQueueCreate(1, sizeof(float));
  xQueuehum = xQueueCreate(1, sizeof(float));
  xQueuetype = xQueueCreate(1, sizeof(int));
  xQueueid = xQueueCreate(1, sizeof(int));
  xQueuedata1 = xQueueCreate(1, sizeof(int));
  xQueuedata2 = xQueueCreate(1, sizeof(int));
  xQueuedata3 = xQueueCreate(1, sizeof(int));
  xQueuetime = xQueueCreate(1, sizeof(int));
  xQueueaddrfreq = xQueueCreate(1, sizeof(int));
  xQueueaddracce = xQueueCreate(1, sizeof(int));
  xQueueaddrdece = xQueueCreate(1, sizeof(int));
  xQueueaddrrh = xQueueCreate(1, sizeof(int));
  xQueueaddrrm = xQueueCreate(1, sizeof(int));
  xQueueaddrrl = xQueueCreate(1, sizeof(int));
  xQueuefreq = xQueueCreate(1, sizeof(int));
  xQueueacce = xQueueCreate(1, sizeof(int));
  xQueuedece = xQueueCreate(1, sizeof(int));
  xQueuerh = xQueueCreate(1, sizeof(int));
  xQueuerm = xQueueCreate(1, sizeof(int));
  xQueuerl = xQueueCreate(1, sizeof(int));
  xQueuenum = xQueueCreate(1, sizeof(int));
  xQueueaddress_ = xQueueCreate(1, sizeof(int));
  xQueueval_ = xQueueCreate(1, sizeof(int));
  xQueuequest = xQueueCreate(1, sizeof(int));
  xQueuehz = xQueueCreate(1, sizeof(int));
  xQueuev = xQueueCreate(1, sizeof(int));
  xQueuea = xQueueCreate(1, sizeof(int));
  xQueuer = xQueueCreate(1, sizeof(int));
  xQueueslave = xQueueCreate(1, sizeof(int));
  xQueuefunc = xQueueCreate(1, sizeof(int));
  xQueueidd = xQueueCreate(1, sizeof(int));
  xQueueaddress = xQueueCreate(1, sizeof(int));
  xQueuequantity = xQueueCreate(1, sizeof(int));
  xQueuevalue = xQueueCreate(1, sizeof(int));
  xQueuepage = xQueueCreate(1, sizeof(String));
  xQueuebtn = xQueueCreate(1, sizeof(String));
  xTaskCreatePinnedToCore(numpage, "numpage", 7000, NULL, 1, &xPageHandle, 0);
}

void loop()
{
  vTaskDelete(NULL);
}

void status(void *pvParameters)
{
  (void)pvParameters;
  WiFiClientSecure client1;
  client1.setCACert(test_root_ca);
  int givin1, givin2, givin3, out1, out2, hyphen1;
  float givtem, givhum;
  String status_getpage, getout1, getout2;
  Serial.println("Board Status");
  for (;;)
  {
    status_flag = true;
    xQueueReceive(xQueuein1, &givin1, 20);
    xQueueReceive(xQueuein2, &givin2, 20);
    xQueueReceive(xQueuein3, &givin3, 20);
    xQueueReceive(xQueuetem, &givtem, 20);
    xQueueReceive(xQueuehum, &givhum, 20);
    String getval = connect_client1(givin1, givin2, givin3, givtem, givhum);
    if (getval != "")
    {
      int v = getval.length();
      int z = 0;
      for (int i = 0; i < v; i++)
      {
        if (getval[i] == '*')
        {
          z++;
        }
        if (z == 1)
        {
          if (getval[i] == '-')
          {
            hyphen1++;
          }
          else
          {
            if (hyphen1 == 0 && getval[i] != '*')
            {
              getout1 = getval[i];
            }
            else if (hyphen1 == 1)
            {
              getout2 = getval[i];
            }
            else if (hyphen1 == 2)
            {
              status_getpage = getval[i];
            }
          }
        }
      }
      hyphen1 = 0;
    }
    if (status_getpage != "1" && status_getpage != "")
    {
      status_flag = false;
      Serial.println("Board Status Stop");
      vTaskResume(xPageHandle);
      vTaskDelete(xBoardHandle);
      vTaskDelete(xStatusHandle);
    }
    out1 = change_string_to_int(getout1);
    out2 = change_string_to_int(getout2);
    xQueueOverwrite(xQueueout1, &out1);
    xQueueOverwrite(xQueueout2, &out2);
    if (status_getpage != "1" && status_getpage != "")
    {
      status_flag = false;
      Serial.println("Board Status Stop");
      vTaskResume(xPageHandle);
      vTaskDelete(xBoardHandle);
      vTaskDelete(xStatusHandle);
    }
    out1 = change_string_to_int(getout1);
    out2 = change_string_to_int(getout2);
    xQueueOverwrite(xQueueout1, &out1);
    xQueueOverwrite(xQueueout2, &out2);
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

void board(void *pvParameters)
{
  (void)pvParameters;
  dht11 DHT11;
  pinMode(25, INPUT_PULLUP);
  pinMode(26, INPUT_PULLUP);
  pinMode(33, INPUT_PULLUP);
  pinMode(27, OUTPUT);
  pinMode(14, OUTPUT);
  float tem, hum;
  int in1, in2, in3, givout1, givout2, getdht;
  for (;;)
  {
    getdht = DHT11.read(DHT11PIN);
    in1 = digitalRead(33);
    in2 = digitalRead(25);
    in3 = digitalRead(26);
    tem = DHT11.temperature;
    hum = DHT11.humidity;
    xQueueOverwrite(xQueuein1, &in1);
    xQueueOverwrite(xQueuein2, &in2);
    xQueueOverwrite(xQueuein3, &in3);
    xQueueOverwrite(xQueuetem, &tem);
    xQueueOverwrite(xQueuehum, &hum);
    xQueueReceive(xQueueout1, &givout1, 20);
    xQueueReceive(xQueueout2, &givout2, 20);
    digitalWrite(27, givout1);
    digitalWrite(14, givout2);
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

void numpage(void *pvParameters)
{
  (void)pvParameters;
  int hyphen2;
  String btn, numbpage;
  Serial.println("Numbpage");
  for (;;)
  {
    String getbtn = connnect_client2();
    if (getbtn != "")
    {
      int h = getbtn.length();
      int z = 0;
      for (int i = 0; i < h; i++)
      {
        if (getbtn[i] == '*')
        {
          z++;
        }
        if (z == 1)
        {
          if (getbtn[i] == '-')
          {
            hyphen2++;
          }
          else
          {
            if (hyphen2 == 0 && getbtn[i] != '*')
            {
              numbpage = getbtn[i];
            }
            else if (hyphen2 == 1)
            {
              btn = getbtn[i];
            }
          }
        }
      }
      hyphen2 = 0;
    }
    if (numbpage == "0")
    {
      digitalWrite(4, !digitalRead(4));
      vTaskDelay(500 / portTICK_PERIOD_MS);
    }
    else if (numbpage == "1" && status_flag == false)
    {
      digitalWrite(4, LOW);
      status_flag = true;
      xTaskCreatePinnedToCore(status, "status", 6000, NULL, 2, &xStatusHandle, 0);
      xTaskCreatePinnedToCore(board, "board", 2000, NULL, 1, &xBoardHandle, 1);
    }
    else if (numbpage == "2" && getdatamb_flag == false)
    {
      digitalWrite(4, LOW);
      getdatamb_flag = true;
      xTaskCreatePinnedToCore(getdata, "getdata", 6000, NULL, 1, &xGetdataHandle, 0);
    }
    else if (numbpage == "3")
    {
      digitalWrite(4, LOW);
      if (filemanager_flag == false)
      {
        filemanager_flag = true;
        xTaskCreatePinnedToCore(filemanager, "filemanager", 6000, NULL, 2, &xFilemanager, 0);
      }
      if (filemanager_flag == true && mbsavedata_flag == true && task_flag == false)
      {
        task_flag = true;
        Serial.println("Mbsavedata Resumed");
        vTaskResume(xMbsaveHandle);
        if (timee_flag == true)
        {
          vTaskResume(xTimeHandle);
        }
        vTaskResume(xFilemanager);
      }
    }
    else if (numbpage == "4" && getdatafr_flag == false)
    {
      digitalWrite(4, LOW);
      getdatafr_flag = true;
      xTaskCreatePinnedToCore(getdatafr, "getdatafr", 6000, NULL, 1, &xGetdatafrHandle, 0);
    }
    else if (numbpage == "5" && setting_flag == false)
    {
      digitalWrite(4, LOW);
      setting_flag = true;
      xTaskCreatePinnedToCore(setting, "setting", 8000, NULL, 1, &xSettingHandle, 0);
    }
    if (numbpage != "" && numbpage != "0")
      vTaskSuspend(xPageHandle);
  }
}

void getdata(void *pvParameters)
{
  (void)pvParameters;
  int hyphen, func, idd, address, quantity, value;
  String data1, data2, data3, data4, data5, getdata_page;
  Serial.println("Getting data from Webserver");
  for (;;)
  {
    getdatamb_flag = true;
    String line = connect_client3();
    if (line != "")
    {
      int n = line.length();
      int z = 0;
      for (int i = 0; i < n; i++)
      {
        if (line[i] == '*')
        {
          z++;
        }
        if (z == 1)
        {
          if (line[i] == '-')
          {
            hyphen++;
          }
          else
          {
            if (hyphen == 0 && line[i] != '*')
            {
              data1 += line[i];
            }
            else if (hyphen == 1)
            {
              data2 += line[i];
            }
            else if (hyphen == 2)
            {
              data3 += line[i];
            }
            else if (hyphen == 3)
            {
              data4 += line[i];
            }
            else if (hyphen == 4)
            {
              data5 += line[i];
            }
            else if (hyphen == 5)
            {
              getdata_page = line[i];
            }
          }
        }
      }
      hyphen = 0;
      func = change_string_to_int(data1);
      idd = change_string_to_int(data2);
      address = change_string_to_int(data3);
      quantity = change_string_to_int(data4);
      value = change_string_to_int(data5);
      data1 = data2 = data3 = data4 = data5 = "";
    }
    if (getdata_page != "2" && getdata_page != "")
    {
      getdatamb_flag = false;
      Serial.println("GetdataMB stopped");
      vTaskResume(xPageHandle);
      vTaskDelete(xGetdataHandle);
    }
    xQueueOverwrite(xQueuefunc, &func);
    xQueueOverwrite(xQueueidd, &idd);
    xQueueOverwrite(xQueueaddress, &address);
    xQueueOverwrite(xQueuequantity, &quantity);
    xQueueOverwrite(xQueuevalue, &value);
    xQueueOverwrite(xQueuepage, &getdata_page);
    if (func != 0 && idd != 0 && mb_flag == false)
    {
      getdatamb_flag = false;
      xTaskCreatePinnedToCore(modbus, "modbus", 5000, NULL, 2, &xModbusHandle, 1);
      vTaskDelete(xGetdataHandle);
    }

    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void modbus(void *pvParameters)
{
  (void)pvParameters;
  int rec_func, rec_idd, rec_address, rec_quantity, rec_value;
  String getnumpage;
  uint8_t u8query1;
  u8query1 = 1;
  Serial.println("Modbus started");
  for (;;)
  {

    mb_flag = true;
    xQueueReceive(xQueuepage, &getnumpage, 50);
    xQueueReceive(xQueuefunc, &rec_func, 50);
    xQueueReceive(xQueueidd, &rec_idd, 50);
    xQueueReceive(xQueueaddress, &rec_address, 50);
    xQueueReceive(xQueuequantity, &rec_quantity, 50);
    xQueueReceive(xQueuevalue, &rec_value, 50);
    value_mb[0] = rec_value;
    if (getnumpage != "2" && mb_flag == true)
    {
      mb_flag = false;
      if (senddata_flag == false)
      {
        Serial.println("Modbus stopped");
        vTaskResume(xPageHandle);
        vTaskDelete(xModbusHandle);
      }
    }
    if (rec_func == 1 || rec_func == 3 || rec_func == 2 || rec_func == 4)
    {
      telegram[0].u8id = rec_idd;
      telegram[0].u8fct = rec_func;
      telegram[0].u16RegAdd = rec_address;
      telegram[0].u16CoilsNo = rec_quantity;
      telegram[0].au16reg = au16data;
      master.query(telegram[0]);
      digitalWrite(18, LOW);
      delay(30);
      digitalWrite(18, HIGH);
      master.poll();
      while (master.getState() != COM_IDLE)
      {
        master.poll();
      }
      if (senddata_flag == false && getnumpage == "2")
      {
        xQueueOverwrite(xQueuefunc, &rec_func);
        xQueueOverwrite(xQueuequantity, &rec_quantity);
        xQueueOverwrite(xQueueaddress, &rec_address);
        xTaskCreatePinnedToCore(senddata, "senddata", 7000, NULL, 2, &xSendHandle, 0);
      }
      digitalWrite(5, LOW);
      delay(30);
      digitalWrite(5, HIGH);
      vTaskDelay(500 / portTICK_PERIOD_MS);
    }
    else
    {
      telegram[1].u8id = rec_idd;
      telegram[1].u8fct = rec_func;
      telegram[1].u16RegAdd = rec_address;
      telegram[1].u16CoilsNo = 1;
      telegram[1].au16reg = value_mb;
      if (rec_func == 5)
      {
        telegram[2].u8id = rec_idd;
        telegram[2].u8fct = 1;
        telegram[2].u16RegAdd = rec_address;
        telegram[2].u16CoilsNo = 1;
        telegram[2].au16reg = au16data;
      }
      else
      {
        telegram[2].u8id = rec_idd;
        telegram[2].u8fct = 3;
        telegram[2].u16RegAdd = rec_address;
        telegram[2].u16CoilsNo = 1;
        telegram[2].au16reg = au16data;
      }
      master.query(telegram[u8query1]);
      master.poll();
      digitalWrite(18, LOW);
      delay(30);
      digitalWrite(18, HIGH);
      while (master.getState() != COM_IDLE)
      {
        master.poll();
      }
      u8query1++;
      if (u8query1 > 2)
      {
        u8query1 = 1;
        if (senddata_flag == false && getnumpage == "2")
        {
          xQueueOverwrite(xQueuefunc, &rec_func);
          xTaskCreatePinnedToCore(senddata, "senddata", 7000, NULL, 2, &xSendHandle, 0);
        }
        digitalWrite(5, LOW);
        delay(30);
        digitalWrite(5, HIGH);
        vTaskDelay(500 / portTICK_PERIOD_MS);
      }
    }
  }
}

void senddata(void *pvParameters)
{
  (void)pvParameters;
  int giv_func, giv_idd, giv_address, giv_quantity, giv_value, hyphen4;
  String data11, data22, data33, data44, data55, data_page;
  xQueueReceive(xQueuefunc, &giv_func, 20);
  xQueueReceive(xQueueaddress, &giv_address, 20);
  xQueueReceive(xQueuequantity, &giv_quantity, 20);
  Serial.println("Sendding data");
  for (;;)
  {
    senddata_flag = true;
    String line = connect_client4(giv_func, giv_address, giv_quantity);
    if (line != "")
    {
      int n = line.length();
      int z = 0;
      for (int i = 0; i < n; i++)
      {
        if (line[i] == '*')
        {
          z++;
        }
        if (z == 1)
        {
          if (line[i] == '-')
          {
            hyphen4++;
          }
          else
          {
            if (hyphen4 == 0 && line[i] != '*')
            {
              data11 += line[i];
            }
            else if (hyphen4 == 1)
            {
              data22 += line[i];
            }
            else if (hyphen4 == 2)
            {
              data33 += line[i];
            }
            else if (hyphen4 == 3)
            {
              data44 += line[i];
            }
            else if (hyphen4 == 4)
            {
              data55 += line[i];
            }
            else if (hyphen4 == 5)
            {
              data_page = line[i];
            }
          }
        }
      }
      hyphen4 = 0;
      giv_func = change_string_to_int(data11);
      giv_idd = change_string_to_int(data22);
      giv_address = change_string_to_int(data33);
      giv_quantity = change_string_to_int(data44);
      giv_value = change_string_to_int(data55);
      data11 = data22 = data33 = data44 = data55 = "";
    }
    xQueueOverwrite(xQueuepage, &data_page);
    xQueueOverwrite(xQueuefunc, &giv_func);
    xQueueOverwrite(xQueueidd, &giv_idd);
    xQueueOverwrite(xQueueaddress, &giv_address);
    xQueueOverwrite(xQueuequantity, &giv_quantity);
    xQueueOverwrite(xQueuevalue, &giv_value);
    if (data_page != "2" && mb_flag == false && data_page != "")
    {
      senddata_flag = false;
      Serial.println("Senddata stopped");
      vTaskDelete(xSendHandle);
    }
    vTaskDelay(200 / portTICK_PERIOD_MS);
  }
}

void filemanager(void *pvParameters)
{
  (void)pvParameters;
  String file_num, file_btn, file_type, file_id, file_data1, file_data2, file_data3, file_time, givp;
  int get_num, get_btn, get_type, get_id, get_data1, get_data2, get_data3, get_time, hyphen3, u;
  int old_data1[3], old_data2[10];
  Serial.println("Filemanager");
  for (;;)
  {
    filemanager_flag = true;
    String line = connect_client5(0, 0, 0);
    if (line != "")
    {
      int n = line.length();
      int j = 0;
      for (int i = 0; i < n; i++)
      {
        if (line[i] == '*')
        {
          j++;
        }
        if (j == 1)
        {
          if (line[i] == '-')
          {
            hyphen3++;
          }
          else
          {
            if (hyphen3 == 0 && line[i] != '*')
            {
              file_num += line[i];
            }
            else if (hyphen3 == 1)
            {
              file_btn += line[i];
            }
            else if (hyphen3 == 2)
            {
              file_type += line[i];
            }
            else if (hyphen3 == 3)
            {
              file_id += line[i];
            }
            else if (hyphen3 == 4)
            {
              file_data1 += line[i];
            }
            else if (hyphen3 == 5)
            {
              file_data2 += line[i];
            }
            else if (hyphen3 == 6)
            {
              file_data3 += line[i];
            }
            else if (hyphen3 == 7)
            {
              file_time += line[i];
            }
          }
        }
      }
      hyphen3 = 0;
      get_num = change_string_to_int(file_num);
      get_btn = change_string_to_int(file_btn);
      get_type = change_string_to_int(file_type);
      get_id = change_string_to_int(file_id);
      get_data1 = change_string_to_int(file_data1);
      get_data2 = change_string_to_int(file_data2);
      get_data3 = change_string_to_int(file_data3);
      get_time = change_string_to_int(file_time);
      file_num = file_btn = file_type = file_id = file_data1 = file_data2 = file_data3 = file_time = "";
    }
    if (get_btn == 1 && mbsavedata_flag == false)
    {
      xQueueOverwrite(xQueuetype, &get_type);
      xQueueOverwrite(xQueueid, &get_id);
      xQueueOverwrite(xQueuedata1, &get_data1);
      xQueueOverwrite(xQueuedata2, &get_data2);
      xQueueOverwrite(xQueuedata3, &get_data3);
      xTaskCreatePinnedToCore(mbsavedata, "mbsavedata", 5000, NULL, 2, &xMbsaveHandle, 1);
      if (get_time != 0)
      {
        xQueueOverwrite(xQueuetime, &get_time);
        timee_flag = true;
        xTaskCreatePinnedToCore(timming, "timming", 7000, NULL, 3, &xTimeHandle, 0);
      }
    }
    else if ((get_btn == 0 || (get_time != 0 && timee_flag == false)) && mbsavedata_flag == true)
    {
      mbsavedata_flag = false;
      mbsave_flag = false;
      for (int i = 0; i < 10; i++)
      {
        old_data2[i] = 0;
      }
      old_data1[0] = old_data1[1] = old_data1[2] = 0;
      Serial.println("MB save stopped");
      vTaskDelete(xMbsaveHandle);
      if (timee_flag == true)
      {
        Serial.println("Time stopped");
        timee_flag = false;
        vTaskDelete(xTimeHandle);
      }
    }
    if (get_num != 3)
    {
      if (get_btn == 1)
      {
        task_flag = false;
        Serial.println("MB save suspend");
        vTaskSuspend(xMbsaveHandle);
        if (timee_flag == true)
        {
          Serial.println("Time suspend");
          vTaskSuspend(xTimeHandle);
        }
        vTaskResume(xPageHandle);
        vTaskSuspend(xFilemanager);
      }
      else
      {
        filemanager_flag = false;
        Serial.println("Filemanager stopped");
        vTaskResume(xPageHandle);
        vTaskDelete(xFilemanager);
      }
    }
    if (mbsave_flag == true)
    {
      if (get_type == 3)
      {
        for (int i = 0; i < 3; i++)
        {
          if (old_data1[i] != datafr[i])
          {
            old_data1[i] = datafr[i];
            u = 1;
          }
        }
        if (u == 1)
        {
          String h = connect_client5(1, 0, 0);
          u = 0;
        }
      }
      else
      {
        for (int i = 0; i < get_data3; i++)
        {
          if (old_data2[i] != dataplc[i])
          {
            String h = connect_client5(2, i, get_data2);
            old_data2[i] = dataplc[i];
          }
        }
      }
    }
    vTaskDelay(1500 / portTICK_PERIOD_MS);
  }
}

void mbsavedata(void *pvParameters)
{
  (void)pvParameters;
  int givtype, givid, givdata1, givdata2, givdata3;
  uint8_t u8query2;
  uint16_t au16datafr[3];
  uint16_t au16dataplc[10];
  u8query2 = 3;
  xQueueReceive(xQueuetype, &givtype, 20);
  xQueueReceive(xQueueid, &givid, 20);
  xQueueReceive(xQueuedata1, &givdata1, 20);
  xQueueReceive(xQueuedata2, &givdata2, 20);
  xQueueReceive(xQueuedata3, &givdata3, 20);
  Serial.println("MB save started");
  for (;;)
  {
    mbsavedata_flag = true;
    task_flag = true;
    if (givtype == 3)
    {
      telegram[3].u8id = givid;
      telegram[3].u8fct = 3;
      telegram[3].u16RegAdd = givdata1;
      telegram[3].u16CoilsNo = 1;
      telegram[3].au16reg = au16datafr;

      telegram[4].u8id = givid;
      telegram[4].u8fct = 3;
      telegram[4].u16RegAdd = givdata2;
      telegram[4].u16CoilsNo = 1;
      telegram[4].au16reg = au16datafr + 1;

      telegram[5].u8id = givid;
      telegram[5].u8fct = 3;
      telegram[5].u16RegAdd = givdata3;
      telegram[5].u16CoilsNo = 1;
      telegram[5].au16reg = au16datafr + 2;
      master.query(telegram[u8query2]);
      master.poll();
      digitalWrite(18, LOW);
      delay(30);
      digitalWrite(18, HIGH);
      while (master.getState() != COM_IDLE)
      {
        master.poll();
      }
      u8query2++;
      if (u8query2 > 5)
      {
        u8query2 = 3;
        datafr[0] = au16datafr[0];
        datafr[1] = au16datafr[1];
        datafr[2] = au16datafr[2];
        mbsave_flag = true;
        digitalWrite(5, LOW);
        delay(30);
        digitalWrite(5, HIGH);
        vTaskDelay(3000 / portTICK_PERIOD_MS);
      }
      else
      {
        vTaskDelay(30 / portTICK_PERIOD_MS);
      }
    }
    else
    {
      telegram[6].u8id = givid;
      telegram[6].u8fct = givdata1;
      telegram[6].u16RegAdd = givdata2;
      telegram[6].u16CoilsNo = givdata3;
      telegram[6].au16reg = au16dataplc;
      master.query(telegram[6]);
      master.poll();
      digitalWrite(18, LOW);
      delay(30);
      digitalWrite(18, HIGH);
      while (master.getState() != COM_IDLE)
      {
        master.poll();
      }
      if (givdata1 == 1 || givdata1 == 2)
      {
        int t = 1;
        for (int i = 0; i < givdata3; i++)
        {
          dataplc[i] = ((au16dataplc[0] & t) >> i);
          t = t * 2;
        }
      }
      else
      {
        for (int i = 0; i < givdata3; i++)
        {
          dataplc[i] = au16dataplc[i];
        }
      }
      mbsave_flag = true;
      vTaskDelay(3000 / portTICK_PERIOD_MS);
    }
  }
}

void timming(void *pvParameters)
{
  (void)pvParameters;
  long givtime, now;
  xQueueReceive(xQueuetime, &givtime, 20);
  now = millis();
  Serial.println("Time is running");
  for (;;)
  {
    timee_flag = true;
    if (millis() - now > (givtime * 60000))
    {
      connect_client6();
      timee_flag = false;
      Serial.println("Time end");
      vTaskDelete(xTimeHandle);
    }
    vTaskDelay(10000 / portTICK_PERIOD_MS);
  }
}

void getdatafr(void *pvParameters)
{
  (void)pvParameters;
  int num_fr, quest_fr, slave_fr, v, a, hz, r, hyphen4;
  String getnum_fr, getquest_fr, getslave_fr, getaddr_v, getaddr_a, getaddr_hz, getaddr_r;
  Serial.println("Getdata FR");
  for (;;)
  {
    getdatafr_flag = true;
    String line = connect_client7();
    if (line != "")
    {
      int n = line.length();
      int z = 0;
      for (int i = 0; i < n; i++)
      {
        if (line[i] == '*')
        {
          z++;
        }
        if (z == 1)
        {
          if (line[i] == '-')
          {
            hyphen4++;
          }
          else
          {
            if (hyphen4 == 0 && line[i] != '*')
            {
              getnum_fr += line[i];
            }
            else if (hyphen4 == 1)
            {
              getquest_fr += line[i];
            }
            else if (hyphen4 == 2)
            {
              getslave_fr += line[i];
            }
            else if (hyphen4 == 3)
            {
              getaddr_v += line[i];
            }
            else if (hyphen4 == 4)
            {
              getaddr_a += line[i];
            }
            else if (hyphen4 == 5)
            {
              getaddr_hz += line[i];
            }
            else if (hyphen4 == 6)
            {
              getaddr_r += line[i];
            }
          }
        }
      }
      hyphen4 = 0;
      num_fr = change_string_to_int(getnum_fr);
      quest_fr = 0;
      slave_fr = change_string_to_int(getslave_fr);
      v = change_string_to_int(getaddr_v);
      a = change_string_to_int(getaddr_a);
      hz = change_string_to_int(getaddr_hz);
      r = change_string_to_int(getaddr_r);
      getnum_fr = getquest_fr = getslave_fr = getaddr_v = getaddr_a = getaddr_hz = getaddr_r = "";
    }
    if (num_fr != 4)
    {
      getdatafr_flag = false;
      Serial.println("Getdata FR Stopped");
      vTaskResume(xPageHandle);
      vTaskDelete(xGetdatafrHandle);
    }
    if (num_fr == 4 && slave_fr != 0)
    {
      getdatafr_flag = false;
      xQueueOverwrite(xQueuenum, &num_fr);
      xQueueOverwrite(xQueuequest, &quest_fr);
      xQueueOverwrite(xQueueslave, &slave_fr);
      xQueueOverwrite(xQueuehz, &hz);
      xQueueOverwrite(xQueuev, &v);
      xQueueOverwrite(xQueuea, &a);
      xQueueOverwrite(xQueuer, &r);
      xTaskCreatePinnedToCore(modbusfr, "modbusfr", 5000, NULL, 2, &xMbfrHandle, 1);
      vTaskDelete(xGetdatafrHandle);
    }
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void modbusfr(void *pvParameters)
{
  (void)pvParameters;
  int getnum_fr, getquest_fr, getslave_fr, gethz_fr, getv_fr, geta_fr, getaddress_, getval_, getr_fr,
      getaddrfreq, getaddracce, getaddrdece, getaddrrh, getaddrrm, getaddrrl,
      getfreq, getacce, getdece, getrh, getrm, getrl;
  uint16_t value_[0], value_1[0], datafreq[0], dataacce[0], datadece[0], datarh[0], datarm[0], datarl[0];
  uint16_t checkdata[6];
  uint8_t u8query3, u8query4, u8query5;
  u8query3 = 11;
  u8query4 = 7;
  u8query5 = 19;
  for (;;)
  {
    mbfr_flag = true;
    xQueueReceive(xQueuequest, &getquest_fr, 20);
    xQueueReceive(xQueueslave, &getslave_fr, 20);
    xQueueReceive(xQueuenum, &getnum_fr, 20);
    if (sendatafr_flag == false && getnum_fr == 4)
    {
      xTaskCreatePinnedToCore(senddatafr, "senddatafr", 10000, NULL, 2, &xSenddatafrHandle, 0);
    }
    if (getnum_fr != 4 && mbfr_flag == true)
    {
      mbfr_flag = false;
      if (sendatafr_flag == false)
      {
        Serial.println("MB FR Stopped");
        vTaskResume(xPageHandle);
        vTaskDelete(xMbfrHandle);
      }
    }
    if (getquest_fr == 0)
    {
      xQueueReceive(xQueuehz, &gethz_fr, 20);
      xQueueReceive(xQueuev, &getv_fr, 20);
      xQueueReceive(xQueuea, &geta_fr, 20);
      xQueueReceive(xQueuer, &getr_fr, 20);
      while (u8query4 < 11)
      {
        telegram[7].u8id = getslave_fr;
        telegram[7].u8fct = 3;
        telegram[7].u16RegAdd = getv_fr;
        telegram[7].u16CoilsNo = 1;
        telegram[7].au16reg = au16datafr;

        telegram[8].u8id = getslave_fr;
        telegram[8].u8fct = 3;
        telegram[8].u16RegAdd = geta_fr;
        telegram[8].u16CoilsNo = 1;
        telegram[8].au16reg = au16datafr + 1;

        telegram[9].u8id = getslave_fr;
        telegram[9].u8fct = 3;
        telegram[9].u16RegAdd = gethz_fr;
        telegram[9].u16CoilsNo = 1;
        telegram[9].au16reg = au16datafr + 2;

        telegram[10].u8id = getslave_fr;
        telegram[10].u8fct = 3;
        telegram[10].u16RegAdd = getr_fr;
        telegram[10].u16CoilsNo = 1;
        telegram[10].au16reg = au16datafr + 3;

        master.query(telegram[u8query4]);
        digitalWrite(18, LOW);
        delay(30);
        digitalWrite(18, HIGH);
        master.poll();
        while (master.getState() != COM_IDLE)
        {
          master.poll();
        }
        u8query4++;
        vTaskDelay(30 / portTICK_PERIOD_MS);
      }
      if (u8query4 > 10)
      {
        u8query4 = 7;
        digitalWrite(5, LOW);
        delay(30);
        digitalWrite(5, HIGH);
      }
    }
    else if (getquest_fr == 1 && sendatafr_flag == true)
    {
      xQueueReceive(xQueueaddress_, &getaddress_, 30);
      xQueueReceive(xQueueval_, &getval_, 30);
      value_[0] = getval_;
      while (flag == false)
      {
        telegram[11].u8id = getslave_fr;
        telegram[11].u8fct = 6;
        telegram[11].u16RegAdd = getaddress_;
        telegram[11].u16CoilsNo = 1;
        telegram[11].au16reg = value_;
        master.query(telegram[11]);
        master.poll();
        digitalWrite(18, LOW);
        delay(30);
        digitalWrite(18, HIGH);
        while (master.getState() != COM_IDLE)
        {
          master.poll();
        }
        digitalWrite(5, LOW);
        delay(30);
        digitalWrite(5, HIGH);
        check_flag = true;
        while (check_flag == true)
        {
          telegram[19].u8id = getslave_fr;
          telegram[19].u8fct = 3;
          telegram[19].u16RegAdd = getaddress_;
          telegram[19].u16CoilsNo = 1;
          telegram[19].au16reg = checkdata;
          master.query(telegram[19]);
          master.poll();
          digitalWrite(18, LOW);
          vTaskDelay(30 / portTICK_PERIOD_MS);
          digitalWrite(18, HIGH);
          while (master.getState() != COM_IDLE)
          {
            master.poll();
          }
          if (value_[0] == checkdata[0])
          {
            Serial.println("mb1 done");
            flag = true;
          }
          digitalWrite(5, LOW);
          delay(30);
          digitalWrite(5, HIGH);
          check_flag = false;
        }
      }
    }
    else if (getquest_fr == 2 && sendatafr_flag == true)
    {
      xQueueReceive(xQueueaddrfreq, &getaddrfreq, 20);
      xQueueReceive(xQueueaddracce, &getaddracce, 20);
      xQueueReceive(xQueueaddrdece, &getaddrdece, 20);
      xQueueReceive(xQueueaddrrh, &getaddrrh, 20);
      xQueueReceive(xQueueaddrrm, &getaddrrm, 20);
      xQueueReceive(xQueueaddrrl, &getaddrrl, 20);
      xQueueReceive(xQueuefreq, &getfreq, 20);
      xQueueReceive(xQueueacce, &getacce, 20);
      xQueueReceive(xQueuedece, &getdece, 20);
      xQueueReceive(xQueuerh, &getrh, 20);
      xQueueReceive(xQueuerm, &getrm, 20);
      xQueueReceive(xQueuerl, &getrl, 20);
      datafreq[0] = getfreq;
      dataacce[0] = getacce;
      datadece[0] = getdece;
      datarh[0] = getrh;
      datarm[0] = getrm;
      datarl[0] = getrl;
      while (flag == false)
      {
        telegram[12].u8id = getslave_fr;
        telegram[12].u8fct = 6;
        telegram[12].u16RegAdd = getaddrfreq;
        telegram[12].u16CoilsNo = 1;
        telegram[12].au16reg = datafreq;

        telegram[13].u8id = getslave_fr;
        telegram[13].u8fct = 6;
        telegram[13].u16RegAdd = getaddracce;
        telegram[13].u16CoilsNo = 1;
        telegram[13].au16reg = dataacce;

        telegram[14].u8id = getslave_fr;
        telegram[14].u8fct = 6;
        telegram[14].u16RegAdd = getaddrdece;
        telegram[14].u16CoilsNo = 1;
        telegram[14].au16reg = datadece;

        telegram[15].u8id = getslave_fr;
        telegram[15].u8fct = 6;
        telegram[15].u16RegAdd = getaddrrh;
        telegram[15].u16CoilsNo = 1;
        telegram[15].au16reg = datarh;

        telegram[16].u8id = getslave_fr;
        telegram[16].u8fct = 6;
        telegram[16].u16RegAdd = getaddrrm;
        telegram[16].u16CoilsNo = 1;
        telegram[16].au16reg = datarm;

        telegram[17].u8id = getslave_fr;
        telegram[17].u8fct = 6;
        telegram[17].u16RegAdd = getaddrrl;
        telegram[17].u16CoilsNo = 1;
        telegram[17].au16reg = datarl;

        master.query(telegram[u8query3]);
        master.poll();
        digitalWrite(18, LOW);
        delay(30);
        digitalWrite(18, HIGH);
        while (master.getState() != COM_IDLE)
        {
          master.poll();
        }
        u8query3++;
        if (u8query3 > 17)
        {
          u8query3 = 12;
          check_flag = true;
          digitalWrite(5, LOW);
          delay(30);
          digitalWrite(5, HIGH);
        }
        else
        {
          vTaskDelay(30 / portTICK_PERIOD_MS);
        }
        while (check_flag == true)
        {
          telegram[20].u8id = getslave_fr;
          telegram[20].u8fct = 3;
          telegram[20].u16RegAdd = getaddrfreq;
          telegram[20].u16CoilsNo = 1;
          telegram[20].au16reg = checkdata;

          telegram[21].u8id = getslave_fr;
          telegram[21].u8fct = 3;
          telegram[21].u16RegAdd = getaddracce;
          telegram[21].u16CoilsNo = 1;
          telegram[21].au16reg = checkdata + 1;

          telegram[22].u8id = getslave_fr;
          telegram[22].u8fct = 3;
          telegram[22].u16RegAdd = getaddrdece;
          telegram[22].u16CoilsNo = 1;
          telegram[22].au16reg = checkdata + 2;

          telegram[23].u8id = getslave_fr;
          telegram[23].u8fct = 3;
          telegram[23].u16RegAdd = getaddrrh;
          telegram[23].u16CoilsNo = 1;
          telegram[23].au16reg = checkdata + 3;

          telegram[24].u8id = getslave_fr;
          telegram[24].u8fct = 3;
          telegram[24].u16RegAdd = getaddrrm;
          telegram[24].u16CoilsNo = 1;
          telegram[24].au16reg = checkdata + 4;

          telegram[25].u8id = getslave_fr;
          telegram[25].u8fct = 3;
          telegram[25].u16RegAdd = getaddrrl;
          telegram[25].u16CoilsNo = 1;
          telegram[25].au16reg = checkdata + 5;

          master.query(telegram[u8query5]);
          master.poll();
          digitalWrite(18, LOW);
          delay(30);
          digitalWrite(18, HIGH);
          while (master.getState() != COM_IDLE)
          {
            master.poll();
          }
          u8query5++;
          if (u8query5 > 25)
          {
            u8query5 = 20;
            if (checkdata[0] == datafreq[0] && checkdata[1] == dataacce[0] && checkdata[2] == datadece[0] &&
                checkdata[3] == datarh[0] && checkdata[4] == datarm[0] && checkdata[5] == datarl[0])
            {
              Serial.println("mb2 done");
              flag = true;
            }
            digitalWrite(5, LOW);
            delay(30);
            digitalWrite(5, HIGH);
            check_flag = false;
          }
          else
          {
            vTaskDelay(30 / portTICK_PERIOD_MS);
          }
        }
      }
    }
    else if (getquest_fr == 3 && sendatafr_flag == true)
    {
      xQueueReceive(xQueueaddress_, &getaddress_, 30);
      xQueueReceive(xQueueval_, &getval_, 30);
      value_1[0] = getval_;
      while (flag == false)
      {
        telegram[18].u8id = getslave_fr;
        telegram[18].u8fct = 6;
        telegram[18].u16RegAdd = getaddress_;
        telegram[18].u16CoilsNo = 1;
        telegram[18].au16reg = value_1;
        master.query(telegram[18]);
        master.poll();
        digitalWrite(18, LOW);
        delay(30);
        digitalWrite(18, HIGH);
        while (master.getState() != COM_IDLE)
        {
          master.poll();
        }
        Serial.println("mb3 done");
        flag = true;
        digitalWrite(5, LOW);
        delay(30);
        digitalWrite(5, HIGH);
      }
    }
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void senddatafr(void *pvParameters)
{
  (void)pvParameters;
  int hyphen5, num, quest, slave, vv, aa, rr, hzz, address_, val_,
      addressfreq, addressacce, addressdece, addressrh, addressrm, addressrl,
      freq, acce, dece, rh, rm, rl;
  String givnum_fr, givquest_fr, givslave_fr, givaddress_fr, givval_fr, givv_fr, giva_fr, givhz_fr, givr_fr,
      givaddressfreq, givaddressacce, givaddressdece, givaddressrh, givaddressrm, givaddressrl,
      givfreq, givacce, givdece, givrh, givrm, givrl;
  for (;;)
  {
    sendatafr_flag = true;
    if (flag == false)
    {
      String line = connect_client8(0);
      if (line != "")
      {
        int n = line.length();
        int z = 0;
        for (int i = 0; i < n; i++)
        {
          if (line[i] == '*')
          {
            z++;
          }
          if (z == 1)
          {
            if (line[i] == '-')
            {
              hyphen5++;
            }
            else
            {
              if (hyphen5 == 0 && line[i] != '*')
              {
                givnum_fr += line[i];
              }
              else if (hyphen5 == 1)
              {
                givquest_fr += line[i];
              }
              else if (hyphen5 == 2)
              {
                givslave_fr += line[i];
              }
              else if (hyphen5 == 3)
              {
                givv_fr += line[i];
              }
              else if (hyphen5 == 4)
              {
                giva_fr += line[i];
              }
              else if (hyphen5 == 5)
              {
                givhz_fr += line[i];
              }
              else if (hyphen5 == 6)
              {
                givr_fr += line[i];
              }
            }
          }
        }
        hyphen5 = 0;
        num = change_string_to_int(givnum_fr);
        quest = change_string_to_int(givquest_fr);
        slave = change_string_to_int(givslave_fr);
        vv = change_string_to_int(givv_fr);
        aa = change_string_to_int(giva_fr);
        hzz = change_string_to_int(givhz_fr);
        rr = change_string_to_int(givr_fr);
        givnum_fr = givquest_fr = givslave_fr = givv_fr = giva_fr = givhz_fr = givr_fr = "";
      }
    }
    xQueueOverwrite(xQueuenum, &num);
    if (num != 4 && mbfr_flag == false)
    {
      sendatafr_flag = false;
      Serial.println("Sendata FR stopped");
      vTaskDelete(xSenddatafrHandle);
    }
    if (quest == 0)
    {
      xQueueOverwrite(xQueuequest, &quest);
      xQueueOverwrite(xQueueslave, &slave);
      xQueueOverwrite(xQueuehz, &hzz);
      xQueueOverwrite(xQueuer, &rr);
    }
    else if (quest == 1 || quest == 3)
    {
      if (flag == false)
      {
        String line = connect_client8(1);
        if (line != "")
        {
          int n = line.length();
          int z = 0;
          for (int i = 0; i < n; i++)
          {
            if (line[i] == '*')
            {
              z++;
            }
            if (z == 1)
            {
              if (line[i] == '-')
              {
                hyphen5++;
              }
              else
              {
                if (hyphen5 == 0 && line[i] != '*')
                {
                  givslave_fr += line[i];
                }
                else if (hyphen5 == 1)
                {
                  givaddress_fr += line[i];
                }
                else if (hyphen5 == 2)
                {
                  givval_fr += line[i];
                }
              }
            }
          }
          hyphen5 = 0;
          address_ = change_string_to_int(givaddress_fr);
          val_ = change_string_to_int(givval_fr);
          givslave_fr = givaddress_fr = givval_fr = "";
        }
      }
      xQueueOverwrite(xQueuequest, &quest);
      xQueueOverwrite(xQueueaddress_, &address_);
      xQueueOverwrite(xQueueval_, &val_);
      if (flag == true)
      {
        String h = connect_client8(2);
        quest = 0;
        xQueueOverwrite(xQueuequest, &quest);
        flag = false;
      }
    }
    else if (quest == 2)
    {
      if (flag == false)
      {
        String line = connect_client8(1);
        if (line != "")
        {
          int n = line.length();
          int z = 0;
          for (int i = 0; i < n; i++)
          {
            if (line[i] == '*')
            {
              z++;
            }
            if (z == 1)
            {
              if (line[i] == '-')
              {
                hyphen5++;
              }
              else
              {
                if (hyphen5 == 0 && line[i] != '*')
                {
                  givslave_fr += line[i];
                }
                else if (hyphen5 == 1)
                {
                  givaddressfreq += line[i];
                }
                else if (hyphen5 == 2)
                {
                  givaddressacce += line[i];
                }
                else if (hyphen5 == 3)
                {
                  givaddressdece += line[i];
                }
                else if (hyphen5 == 4)
                {
                  givaddressrh += line[i];
                }
                else if (hyphen5 == 5)
                {
                  givaddressrm += line[i];
                }
                else if (hyphen5 == 6)
                {
                  givaddressrl += line[i];
                }
                else if (hyphen5 == 7)
                {
                  givfreq += line[i];
                }
                else if (hyphen5 == 8)
                {
                  givacce += line[i];
                }
                else if (hyphen5 == 9)
                {
                  givdece += line[i];
                }
                else if (hyphen5 == 10)
                {
                  givrh += line[i];
                }
                else if (hyphen5 == 11)
                {
                  givrm += line[i];
                }
                else if (hyphen5 == 12)
                {
                  givrl += line[i];
                }
              }
            }
          }
          hyphen5 = 0;
          slave = change_string_to_int(givslave_fr);
          addressfreq = change_string_to_int(givaddressfreq);
          addressacce = change_string_to_int(givaddressacce);
          addressdece = change_string_to_int(givaddressdece);
          addressrh = change_string_to_int(givaddressrh);
          addressrm = change_string_to_int(givaddressrm);
          addressrl = change_string_to_int(givaddressrl);
          freq = change_string_to_int(givfreq);
          acce = change_string_to_int(givacce);
          dece = change_string_to_int(givdece);
          rh = change_string_to_int(givrh);
          rm = change_string_to_int(givrm);
          rl = change_string_to_int(givrl);
          givslave_fr = givaddressfreq = givaddressacce = givaddressdece = givaddressrh = givaddressrm = givaddressrl = givfreq = givacce = givdece = givrh = givrm = givrl = "";
        }
      }
      xQueueOverwrite(xQueuequest, &quest);
      xQueueOverwrite(xQueueaddrfreq, &addressfreq);
      xQueueOverwrite(xQueueaddracce, &addressacce);
      xQueueOverwrite(xQueueaddrdece, &addressdece);
      xQueueOverwrite(xQueueaddrrh, &addressrh);
      xQueueOverwrite(xQueueaddrrm, &addressrm);
      xQueueOverwrite(xQueueaddrrl, &addressrl);
      xQueueOverwrite(xQueuefreq, &freq);
      xQueueOverwrite(xQueueacce, &acce);
      xQueueOverwrite(xQueuedece, &dece);
      xQueueOverwrite(xQueuerh, &rh);
      xQueueOverwrite(xQueuerm, &rm);
      xQueueOverwrite(xQueuerl, &rl);
      if (flag == true)
      {
        String h = connect_client8(2);
        quest = 0;
        xQueueOverwrite(xQueuequest, &quest);
        flag = false;
      }
    }
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

void setting(void *pvParameters)
{
  (void)pvParameters;
  String getmode, getmin, getpage;
  int mode, min, page, hyphen6, o;
  Serial.println("Setting");
  for (;;)
  {
    String line = connect_client9();
    if (line != "")
    {
      int y = line.length();
      int z = 0;
      for (int i = 0; i < y; i++)
      {
        if (line[i] == '*')
        {
          z++;
        }
        if (z == 1)
        {
          if (line[i] == '-')
          {
            hyphen6++;
          }
          else
          {
            if (hyphen6 == 0 && line[i] != '*')
            {
              getmode += line[i];
            }
            else if (hyphen6 == 1)
            {
              getmin += line[i];
            }
            else if (hyphen6 == 2)
            {
              getpage += line[i];
            }
          }
        }
      }
      hyphen6 = 0;
      min = change_string_to_int(getmin);
      mode = change_string_to_int(getmode);
      page = change_string_to_int(getpage);
      getmode = getmin = getpage = "";
    }
    if (page != 5)
    {
      setting_flag = false;
      vTaskResume(xPageHandle);
      vTaskDelete(xSettingHandle);
    }
    if (mode == 1)
    {
      while (o != 10)
      {
        digitalWrite(4, !digitalRead(4));
        vTaskDelay(100 / portTICK_PERIOD_MS);
        digitalWrite(4, !digitalRead(4));
        vTaskDelay(100 / portTICK_PERIOD_MS);
        o++;
      }
      digitalWrite(4, HIGH);
      esp_deep_sleep(min * 1000000 * 60);
      esp_deep_sleep_start();
    }
    else if (mode == 2)
    {
      while (o != 10)
      {
        digitalWrite(4, !digitalRead(4));
        vTaskDelay(100 / portTICK_PERIOD_MS);
        digitalWrite(4, !digitalRead(4));
        vTaskDelay(100 / portTICK_PERIOD_MS);
        o++;
      }
      esp_sleep_enable_ext0_wakeup(GPIO_NUM_13, 0);
      esp_deep_sleep_start();
    }
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void reconnect(void *pvParameters)
{
  (void)pvParameters;
  Serial.println("Try to reconnect");
  for (;;)
  {
    autoconnect_flag = true;
    if (connect_client())
    {
      Serial.print("...");
    }
    else
    {
      digitalWrite(2, HIGH);
      Serial.println("Connect Successful");
      switch (task_number)
      {
      case 1:
        vTaskResume(xBoardHandle);
        vTaskResume(xStatusHandle);
        break;
      case 2:
        vTaskResume(xPageHandle);
        break;
      case 3:
        vTaskResume(xGetdataHandle);
        break;
      case 4:
        vTaskResume(xModbusHandle);
        vTaskResume(xSendHandle);
        break;
      case 5:
        if (mbsavedata_flag == true)
        {
          vTaskResume(xMbsaveHandle);
        }
        if (timee_flag == true)
        {
          vTaskResume(xTimeHandle);
        }
        vTaskResume(xFilemanager);
        break;
      case 6:
        vTaskResume(xTimeHandle);
        break;
      case 7:
        vTaskResume(xGetdatafrHandle);
        break;
      case 8:
        vTaskResume(xSenddatafrHandle);
        vTaskResume(xMbfrHandle);
        break;
      case 9:
        vTaskResume(xSettingHandle);
        break;
      }
      task_number = 0;
      autoconnect_flag = false;
      vTaskDelete(xReconnectHandle);
    }
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

