#include <SdLink.h>

SdLink::SdLink(
    const char* path_read,
    const char* path_write,
    int cs_pin,
    fs::SDFS& sdfs,
    SPIClass& spi,
    Logger& log
) : _path_read(path_read),
    _path_write(path_write),
    _cs_pin(cs_pin),
    _sd(sdfs),
    _spi(spi),
    _log(log)
{
    if (_mtx == NULL) {
        _mtx = xSemaphoreCreateMutex();
    }
}

void SdLink::connect() {
    xSemaphoreTake(_mtx, portMAX_DELAY);
    if (!_init) {
        _sd.begin(_cs_pin);
        _init = true;
    }
    xSemaphoreGive(_mtx);
}

int SdLink::connect(const char* path, uint16_t mode) {
    _path_read = path;
    _path_write = path;
    bool result = _init;
    xSemaphoreTake(_mtx, portMAX_DELAY);
    if (!_init) {
        _init = true;
        result = _sd.begin(_cs_pin);
    }
    xSemaphoreGive(_mtx);
    return result && SdLink::connected();
}

int SdLink::connect(IPAddress, uint16_t) {
    bool result = _init;
    xSemaphoreTake(_mtx, portMAX_DELAY);
    if (!_init) {
        _init = true;
        result = _sd.begin(_cs_pin);
    }
    xSemaphoreGive(_mtx);
    return result && SdLink::connected();
}

size_t SdLink::write(uint8_t byte) {
    if (!SdLink::connected()) {
        return 0;
    }
    xSemaphoreTake(_mtx, portMAX_DELAY);
    File file = _sd.open(_path_write, FILE_APPEND);
    if (!file) {
        _log.error("SDlink", "Failed to open dir %s for write", _path_write);
        xSemaphoreGive(_mtx);
        return 0;
    }
    int result = file.print(byte);
    file.close();
    xSemaphoreGive(_mtx);
    return result;
}

size_t SdLink::write(const uint8_t *buf, size_t size) {
    if (!SdLink::connected()) {
        return 0;
    }
    xSemaphoreTake(_mtx, portMAX_DELAY);
    File file = _sd.open(_path_write, FILE_APPEND);
    if (!file) {
        _log.error("SDlink", "Failed to open dir %s for write", _path_write);
        xSemaphoreGive(_mtx);
        return 0;
    }
    int result = file.write(buf, size);
    file.close();
    xSemaphoreGive(_mtx);
    return result;
}

int SdLink::available() {
    if (!SdLink::connected()) {
        return 0;
    }
    xSemaphoreTake(_mtx, portMAX_DELAY);
    File file = _sd.open(_path_read, FILE_READ);
    if (!file) {
        xSemaphoreGive(_mtx);
        return 0;
    }
    file.close();
    file = _sd.open(_path_write, FILE_READ);
    if (!file) {
        xSemaphoreGive(_mtx);
        return 0;
    }
    file.close();
    xSemaphoreGive(_mtx);
    return 1;
}

int SdLink::read(uint8_t* buffer, size_t size) {
    if (!SdLink::connected()) {
        return 0;
    }
    xSemaphoreTake(_mtx, portMAX_DELAY);
    File file = _sd.open(_path_read, FILE_READ);
    if (!file) {
        _log.error("SDlink", "Failed to open dir %s for read", _path_read);
        xSemaphoreGive(_mtx);
        return 0;
    }
    int result = file.readBytes((char*)buffer, size);
    file.close();
    xSemaphoreGive(_mtx);
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
    xSemaphoreTake(_mtx, portMAX_DELAY);
    _sd.end();
    xSemaphoreGive(_mtx);
}

uint8_t SdLink::connected() {
    return _sd.cardType() != CARD_NONE && _sd.cardType() != CARD_UNKNOWN;
}

SdLink::operator bool() {
    return _sd.cardType() != CARD_NONE && _sd.cardType() != CARD_UNKNOWN;
}