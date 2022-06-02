#include <SdLink.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"

SdLink::SdLink(
    Logger& log
) : _log(log)
{
    if (_mtx == NULL) {
        _mtx = xSemaphoreCreateMutex();
    }
}

void SdLink::connect() {

}

int SdLink::connect(const char*, uint16_t) {
    
    return true;
}

int SdLink::connect(IPAddress, uint16_t) {
    
    return true;
}

size_t SdLink::write(uint8_t byte) {
    
    return false;
}

size_t SdLink::write(const uint8_t *buf, size_t size) {
    
    return false;
}

int SdLink::available() {
    return true;
}

int SdLink::read(uint8_t* buffer, size_t size) {
    
    return 0;
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
    
}

uint8_t SdLink::connected() {
    return true;
}

SdLink::operator bool() {
    return true;
}