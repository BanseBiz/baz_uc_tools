#include <SdLink.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"

SdLink::SdLink(
    const char* path_read,
    const char* path_write,
    fs::SDFS& sdfs,
    SPIClass& spi,
    Logger& log
) : _path_read(path_read),
    _path_write(path_write),
    _sd(sdfs),
    _spi(spi),
    _log(log)
{
    if (_mtx == NULL) {
        _mtx = xSemaphoreCreateMutex();
    }
}

void SdLink::connect() {
    _sd.begin(5);
}

int SdLink::connect(const char* path, uint16_t mode) {
    _path_read = path;
    _path_write = path;
    return _sd.begin(5) && SdLink::connected();
}

int SdLink::connect(IPAddress, uint16_t) {
    return _sd.begin(5) && SdLink::connected();
}

size_t SdLink::write(uint8_t byte) {
    if (!SdLink::connected()) {
        return 0;
    }
    File file = _sd.open(_path_write, FILE_APPEND);
    if (!file) {
        return 0;
    }
    int result = file.print(byte);
    file.close();
    return result;
}

size_t SdLink::write(const uint8_t *buf, size_t size) {
    if (!SdLink::connected()) {
        return 0;
    }
    File file = _sd.open(_path_write, FILE_APPEND);
    if (!file) {
        return 0;
    }
    int result = file.write(buf, size);
    file.close();
    return result;
}

int SdLink::available() {
    if (!SdLink::connected()) {
        return 0;
    }
    File file = _sd.open(_path_read, FILE_READ);
    if (!file) {
        return 0;
    }
    file.close();
    file = _sd.open(_path_write, FILE_READ);
    if (!file) {
        return 0;
    }
    file.close();
    return 1;
}

int SdLink::read(uint8_t* buffer, size_t size) {
    if (!SdLink::connected()) {
        return 0;
    }
    File file = _sd.open(_path_read, FILE_READ);
    if (!file) {
        return 0;
    }
    int result = file.readBytes((char*)buffer, size);
    file.close();
    return result;
}

int SdLink::read() {
    return 0;
}

int SdLink::peek() {
    return 0;
}

void SdLink::flush() {
    
}

void SdLink::stop() {
    _sd.end();
}

uint8_t SdLink::connected() {
    return _sd.cardType() != CARD_NONE;
}

SdLink::operator bool() {
    return _sd.cardType() != CARD_NONE;
}