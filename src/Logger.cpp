#define INCLUDE_vTaskSuspend 1
#define LOGGER_RATE 10
#define LOGGER_STACK 8196

#define LOGGER_HEADER_BUFFER 64

#define LOGGER_LEVEL_DEBUG 3
#define LOGGER_LEVEL_INFO 2
#define LOGGER_LEVEL_WARN 1
#define LOGGER_LEVEL_ERROR 0

#include "Logger.h"
#include "esp_log.h"
#include <string.h>
#include <stdio.h>
#include <sstream>
#include <esp32-hal.h>

Logger::Logger(Client& link) : 
    Worker(LOGGER_RATE, LOGGER_STACK),
    _link(link)
{
    _mtx = xSemaphoreCreateMutex();
}

void Logger::init() {

}

void Logger::debug(const char* tag, const char* msg, ...) {
    #if !defined LOGGER_LEVEL || LOGGER_LEVEL >= LOGGER_LEVEL_DEBUG
    if (tag == "GPS") {
        return;
    }
    va_list vl;
    va_start(vl,msg);
    char message[LOGGER_BUFFER];
    vsprintf(message,msg,vl);
    char buffer[LOGGER_BUFFER];
    sprintf(buffer,"%012lu-%s: %s: %s\n",millis(),"DEBUG",tag,message);
    va_end(vl);
    xSemaphoreTake(_mtx, portMAX_DELAY);
    _log.push_back(std::string(buffer));
    xSemaphoreGive(_mtx);
    #endif
}

void Logger::debug(const char* tag, std::string msg) {
    #if !defined LOGGER_LEVEL || LOGGER_LEVEL >= LOGGER_LEVEL_DEBUG
    if (tag == "GPS") {
        return;
    }
    debug(tag,msg.c_str());
    #endif
}

void Logger::info(const char* tag, const char* msg, ...) {
    #if !defined LOGGER_LEVEL || LOGGER_LEVEL >= LOGGER_LEVEL_INFO
    va_list vl;
    va_start(vl,msg);
    char message[LOGGER_BUFFER];
    vsprintf(message,msg,vl);
    char buffer[LOGGER_BUFFER];
    sprintf(buffer,"%012lu-%s: %s: %s\n",millis(),"_INFO",tag,message);
    va_end(vl);
    xSemaphoreTake(_mtx, portMAX_DELAY);
    _log.push_back(std::string(buffer));
    xSemaphoreGive(_mtx);
    #endif
}

void Logger::warn(const char* tag, const char* msg, ...) {
    #if !defined LOGGER_LEVEL || LOGGER_LEVEL >= LOGGER_LEVEL_WARN
    va_list vl;
    va_start(vl,msg);
    char message[LOGGER_BUFFER];
    vsprintf(message,msg,vl);
    char buffer[LOGGER_BUFFER];
    sprintf(buffer,"%012lu-%s: %s: %s\n",millis(),"_WARN",tag,message);
    va_end(vl);
    xSemaphoreTake(_mtx, portMAX_DELAY);
    _log.push_back(std::string(buffer));
    xSemaphoreGive(_mtx);
    #endif
}

void Logger::error(const char* tag, const char* msg, ...) {
    va_list vl;
    va_start(vl,msg);
    char message[LOGGER_BUFFER];
    vsprintf(message,msg,vl);
    char buffer[LOGGER_BUFFER];
    sprintf(buffer,"%012lu-%s: %s: %s\n",millis(),"ERROR",tag,message);
    va_end(vl);
    xSemaphoreTake(_mtx, portMAX_DELAY);
    _log.push_back(std::string(buffer));
    xSemaphoreGive(_mtx);
}

void Logger::set(unsigned int ind, std::string val) {
    _map[ind] = val;
}

std::string Logger::get(unsigned int ind) {
    return _map[ind];
}

void Logger::loop() {
    xSemaphoreTake(_mtx, portMAX_DELAY);
    std::vector<std::string> log(_log);
    _log.clear();
    xSemaphoreGive(_mtx);

    std::stringstream ss;
    for (auto msg : log) {
        ss << msg;
    }
    
    std::string output = ss.str();
    _link.write((const uint8_t*) output.c_str(), output.size());
}