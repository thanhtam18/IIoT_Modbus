#ifndef __MAIN_H
#define __MAIN_H

#include <ModbusRtu.h>

boolean autoconnect_flag = false,
        global_ = false,
        mb_flag = false,
        senddata_flag = false,
        status_flag = false,
        getdatamb_flag = false,
        getdatafr_flag = false,
        mbsavedata_flag = false,
        task_flag = false,
        filemanager_flag = false,
        sendatafr_flag = false,       
        flag = false,
        mbfr_flag = false,
        timee_flag = false,
        mbsave_flag = false,
        check_flag = false,
        setting_flag = false;

TaskHandle_t xModbusHandle, xSendHandle, xGetdataHandle, xStatusHandle, xFilemanager, xBoardHandle, xSettingHandle,
    xMbsaveHandle, xDatasaveHandle, xPageHandle, xReconnectHandle, xTimeHandle, xGetdatafrHandle, xMbfrHandle, xSenddatafrHandle;
    
QueueHandle_t xQueuebtn, xQueuepage, xQueuefunc, xQueueidd, xQueueaddress, xQueuequantity, xQueuevalue, xQueuequest, xQueueslave, xQueuehz,
    xQueuev, xQueuea, xQueuer, xQueuenum, xQueueaddress_, xQueueval_, xQueueaddrfreq, xQueueaddracce, xQueueaddrdece, xQueueaddrrh, xQueueaddrrm,
    xQueueaddrrl, xQueuefreq, xQueueacce, xQueuedece, xQueuerh, xQueuerm, xQueuerl, xQueuetype, xQueueid, xQueuedata1,
    xQueuedata2, xQueuedata3, xQueuetime, xQueuein1, xQueuein2, xQueuein3, xQueueout1, xQueueout2, xQueuetem, xQueuehum;


Modbus master(0, Serial2, 0);
modbus_t telegram[28];
uint16_t value_mb[1];
uint16_t au16data[16];
uint16_t au16datafr[5];
int datafr[3], dataplc[10];
int getport;
const char *host;
int httpPort;
String myhost = "";
String myport = "";
String ssid = "";
String pass = "";

int change_string_to_int(String s);
void numpage(void *pvParameters);
void board(void *pvParameters);
void status(void *pvParameters);
void getdata(void *pvParameters);
void modbus(void *pvParameters);
void senddata(void *pvParameters);
void filemanager(void *pvParameters);
void getdatafr(void *pvParameters);
void setting(void *pvParameters);
void mbsavedata(void *pvParameters);
void timming(void *pvParameters);
void modbusfr(void *pvParameters);
void senddatafr(void *pvParameters);
void reconnect(void *pvParameters);

#endif