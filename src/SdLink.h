#pragma once

#include <Client.h>
#include <Logger.h>

class SdLink : public Client
{
private:
    Logger& _log;
    SemaphoreHandle_t _mtx;
public:
    SdLink(Logger&);
    void connect();
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
};