#define INCLUDE_vTaskSuspend 1

#include "SerialLink.h"

SemaphoreHandle_t SerialLink::_mtx = NULL;;

SerialLink::SerialLink(
    HardwareSerial& serial,
    unsigned long baud
) : _serial(serial),
    _baud(baud)
{
    if (_mtx == NULL) {
        _mtx = xSemaphoreCreateMutex();
    }
}

void SerialLink::connect() {
    xSemaphoreTake(_mtx, portMAX_DELAY);
    _connected = true;
    _serial.begin(_baud);
    xSemaphoreGive(_mtx);
}

int SerialLink::connect(const char*, uint16_t) {
    xSemaphoreTake(_mtx, portMAX_DELAY);
    _connected = true;
    _serial.begin(_baud);
    xSemaphoreGive(_mtx);
    return true;
}

int SerialLink::connect(IPAddress, uint16_t) {
    xSemaphoreTake(_mtx, portMAX_DELAY);
    _connected = true;
    _serial.begin(_baud);
    xSemaphoreGive(_mtx);
    return true;
}

size_t SerialLink::write(uint8_t byte) {
    xSemaphoreTake(_mtx, portMAX_DELAY);
    if (_serial.availableForWrite()) {
        int ret = _serial.write(byte);
        _serial.flush();
        xSemaphoreGive(_mtx);
        return ret;
    }
    xSemaphoreGive(_mtx);
    return false;
}

size_t SerialLink::write(const uint8_t *buf, size_t size) {
    xSemaphoreTake(_mtx, portMAX_DELAY);
    if (_serial.availableForWrite()) {
        int ret = _serial.write(buf, size);
        _serial.flush();
        xSemaphoreGive(_mtx);
        return ret;
    }
    xSemaphoreGive(_mtx);
    return false;
}

int SerialLink::available() {
    return _serial.available();
}

int SerialLink::read(uint8_t* buffer, size_t size) {
    xSemaphoreTake(_mtx, portMAX_DELAY);
    if (_serial.available()) {
        int ret = _serial.read(buffer, size);
        xSemaphoreGive(_mtx);
        return ret;
    }
    xSemaphoreGive(_mtx);
    return 0;
}

int SerialLink::read() {
    xSemaphoreTake(_mtx, portMAX_DELAY);
    if (_serial.available()) {
        int ret = _serial.read();
        xSemaphoreGive(_mtx);
        return ret;
    }
    xSemaphoreGive(_mtx);
    return 0;
}

int SerialLink::peek() {
    return _serial.peek();
}

void SerialLink::flush() {
    xSemaphoreTake(_mtx, portMAX_DELAY);
    _serial.flush();
    xSemaphoreGive(_mtx);
}

void SerialLink::stop() {
    xSemaphoreTake(_mtx, portMAX_DELAY);
    _connected = false;
    _serial.end();
    xSemaphoreGive(_mtx);
}

uint8_t SerialLink::connected() {
    return _connected;
}

SerialLink::operator bool() {
    return _connected;
}