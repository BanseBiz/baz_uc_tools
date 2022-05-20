#pragma once

#include <Worker.h>
#include <Client.h>
#include <vector>
#include <FreeRTOS.h>
#include <Logger.h>

#define SINK_STATUS_ID 0x02
#define SINK_MAX_MSGS_PENDING 48

class Sink : public Worker {
  public:
    Sink(const char*, uint16_t, Client&, Logger&);
    void init() override;
    bool write(std::string&);
    bool write(const char*, size_t);
  private:
    void loop() override;
    std::vector<std::string> _data;
    std::vector<std::string> _pending;
    Client& _uplink;
    Logger& _log;
    SemaphoreHandle_t _mtx;
    size_t _stack_size = 0;
    const char* _host;
    const uint16_t _port;
};