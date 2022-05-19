#pragma once

#include <WiFi.h>
#include "Logger.h"
#include "Worker.h"
#include "FreeRTOS.h"

#define WIFI_STATUS_ID 0x00

class WifiMgr : public Worker
{
private:
    const char* _ssid;
    const char* _key;
    SemaphoreHandle_t _mtx;
    Logger& _log;
    void loop() override;
public:
    WifiMgr(const char*, const char*, Logger&);
    void init() override;
    bool isUp();
};