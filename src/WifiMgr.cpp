#define INCLUDE_vTaskSuspend 1
#define WIFI_STACK_SIZE 8196
#define WIFI_RATE 0.25f //hz

#include "WifiMgr.h"

WifiMgr::WifiMgr(const char* ssid, const char* key, Logger& log
) : Worker(WIFI_RATE,WIFI_STACK_SIZE), _ssid(ssid), _key(key), _log(log)
{

}

void WifiMgr::init() {
    WiFi.begin(_ssid, _key);
}

void WifiMgr::loop() {
    switch ( WiFi.status() ) {
        case WL_NO_SHIELD: 
        {
            _log.error("WIFI","no module");
            return;
        }
        case WL_IDLE_STATUS:
        {
            WiFi.begin(_ssid, _key);
            return;
        }
        case WL_NO_SSID_AVAIL:
        {
            _log.warn("WIFI","no ssid");
            WiFi.begin(_ssid, _key);
            return;
        }
        case WL_SCAN_COMPLETED: 
            _log.info("WIFI","scan completed");
            return;
        case WL_CONNECTED: 
            _log.debug("WIFI","connected");
            return;
        case WL_CONNECT_FAILED:
        { 
            _log.warn("WIFI","connection failed");
            WiFi.begin(_ssid, _key);
            return;
        }
        case WL_CONNECTION_LOST: 
        {
            _log.warn("WIFI","connection lost");
            WiFi.begin(_ssid, _key);
            return;
        }
        case WL_DISCONNECTED: 
        {
            _log.warn("WIFI","disconnected");
            WiFi.begin(_ssid, _key);
            return;
        }
    }
}

bool WifiMgr::isUp() {
    return WiFi.isConnected();
}