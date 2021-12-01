#pragma once

#define LOGGER_BUFFER 256
#define LOGGER_LEVEL 2

#include <Client.h>
#include <string.h>
#include <vector>
#include "Worker.h"

class Logger : public Worker {
  public:
    typedef enum loglevel_t {
      l_debug = 0x00,
      l_info = 0x01,
      l_warn = 0x02,
      l_error = 0x03
    } loglevel_t;
    Logger(Client&);
    void init() override;
    void debug(const char*, const char*, ...);
    void debug(const char*, std::string);
    void info(const char*, const char*, ...);
    void warn(const char*, const char*, ...);
    void error(const char*, const char*, ...);
  private:
    std::vector<std::string> _log;
    Client& _link;
    SemaphoreHandle_t _mtx;
    void loop() override;
};