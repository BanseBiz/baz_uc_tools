#include <Sink.h>
#include <sstream>

#define SINK_RATE 0.1f
#define SINK_STACK_SIZE 8192

Sink::Sink(const char* host, uint16_t port, Client& client, Logger& log)
    : Worker(SINK_RATE,SINK_STACK_SIZE),
     _uplink(client),
     _log(log),
     _host(host),
     _port(port)
{
    _mtx = xSemaphoreCreateMutex();
}

void Sink::init()  {

}

bool Sink::write(std::string& msg) {
    msg.append("\n");
    xSemaphoreTake(_mtx, portMAX_DELAY);
    _data.push_back(msg);
    xSemaphoreGive(_mtx);
    return true;
}

bool Sink::write(const char* data, size_t size) {
    std::string msg(data, size);
    return write(msg);
}

void Sink::loop() {
    xSemaphoreTake(_mtx, portMAX_DELAY);
    std::vector<std::string> pending(_data);
    std::stringstream ss;
    for (auto msg : _data) {
        ss << msg;
    }
    _data.clear();
    xSemaphoreGive(_mtx);
    _pending.push_back(ss.str());
    if (!_uplink && !_uplink.connect(_host, _port)) {
        _log.warn("SINK","offline, %d messages pending",_pending.size());
        return;
    }
    for (auto msg : _pending) {
        _uplink.write((const uint8_t*) msg.c_str(), msg.size());
    }
    _pending.clear();
}

