#pragma once

#include <WiFi.h>
#include <Client.h>
#include <WifiMgr.h>
#include "Logger.h"
#include "FreeRTOS.h"

class WifiLink : public Client
{
private:
    WiFiClient _wifi;
    WifiMgr& _mgr;
    Logger& _log;
    SemaphoreHandle_t _mtx;
public:
    WifiLink(WifiMgr&, Logger&);
    int connect(const char*, uint16_t) override;
    int connect(IPAddress ip, uint16_t port) override;
    size_t write(uint8_t) override;
    size_t write(const uint8_t *buf, size_t size) override;
    int available() override;
    int read() override;
    int read(uint8_t*, size_t) override;
    int peek() override;
    void flush() override;
    void stop() override;
    uint8_t connected() override;
    operator bool() override;
    WiFiClient& getClient();
};
