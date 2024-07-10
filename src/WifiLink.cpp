#define INCLUDE_vTaskSuspend 1
#define WIFI_STACK_SIZE 8196
#define WIFI_RATE 0.25f //hz

#include "WifiLink.h"

//SemaphoreHandle_t WifiLink::_mtx = NULL;

WifiLink::WifiLink(
    WifiMgr& mgr,
    Logger& log
) : _mgr(mgr), _log(log)
{
    if (_mtx == NULL) {
        _mtx = xSemaphoreCreateMutex();
    }
}

int WifiLink::connect(const char* host, uint16_t port) {
    if (!_mgr.isUp()) {
        return false;
    }
    xSemaphoreTake(_mtx, portMAX_DELAY);
    int ret = _wifi.connect(host, port);
    xSemaphoreGive(_mtx);
    return ret;
}

int WifiLink::connect(IPAddress ip, uint16_t port) {
    if (!_mgr.isUp()) {
        return false;
    }
    xSemaphoreTake(_mtx, portMAX_DELAY);
    int ret = _wifi.connect(ip, port);
    xSemaphoreGive(_mtx);
    return ret;
}

size_t WifiLink::write(uint8_t byte) {
    xSemaphoreTake(_mtx, portMAX_DELAY);
    int ret = _wifi.write(byte);
    xSemaphoreGive(_mtx);
    return ret;
}

size_t WifiLink::write(const uint8_t* buffer, size_t size) {
    xSemaphoreTake(_mtx, portMAX_DELAY);
    int ret = _wifi.write(buffer, size);
    xSemaphoreGive(_mtx);
    return ret;
}

int WifiLink::available() {
    xSemaphoreTake(_mtx, portMAX_DELAY);
    int ret = _wifi.available();
    xSemaphoreGive(_mtx);
    return ret;
}

int WifiLink::read() {
    xSemaphoreTake(_mtx, portMAX_DELAY);
    if (_wifi.available()) {
        int ret = _wifi.read();
        xSemaphoreGive(_mtx);
        return ret;
    }
    xSemaphoreGive(_mtx);
    return false;
}

int WifiLink::read(uint8_t* buffer, size_t size) {
    xSemaphoreTake(_mtx, portMAX_DELAY);
    if (_wifi.available()) {
        int ret = _wifi.read(buffer, size);
        xSemaphoreGive(_mtx);
        return ret;
    }
    xSemaphoreGive(_mtx);
    return false;
}

int WifiLink::peek() {
    xSemaphoreTake(_mtx, portMAX_DELAY);
    if (_wifi.available()) {
        int ret = _wifi.peek();
        xSemaphoreGive(_mtx);
        return ret;
    }
    xSemaphoreGive(_mtx);
    return false;
}

void WifiLink::flush() {
    xSemaphoreTake(_mtx, portMAX_DELAY);
    _wifi.flush();
    xSemaphoreGive(_mtx);
}

void WifiLink::stop() {
    xSemaphoreTake(_mtx, portMAX_DELAY);
    _wifi.stop();
    xSemaphoreGive(_mtx);
}

uint8_t WifiLink::connected() {
    return _mgr.isUp() && _wifi.connected();
}

WifiLink::operator bool() {
    return _mgr.isUp() && _wifi.connected();
}

WiFiClient& WifiLink::getClient() {
    return _wifi;
}