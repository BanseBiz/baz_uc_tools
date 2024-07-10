#define INCLUDE_vTaskSuspend 1
#define WIFI_STACK_SIZE 8196
#define WIFI_RATE .1f //hz

#include "WifiMgr.h"

WifiMgr::WifiMgr(const char* ssid, const char* key, Logger& log
) : Worker(WIFI_RATE,WIFI_STACK_SIZE), _ssid(ssid), _key(key), _log(log)
{

}

void WifiMgr::init() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(_ssid, _key);
}

void WifiMgr::loop() {
    switch ( WiFi.status() ) {
        case WL_NO_SHIELD: 
        {
            _log.error("WIFI","no module");
            _log.set(WIFI_STATUS_ID,"no module");
            return;
        }
        case WL_IDLE_STATUS:
        {
            WiFi.mode(WIFI_STA);
            WiFi.begin(_ssid, _key);
            return;
        }
        case WL_NO_SSID_AVAIL:
        {
            _log.warn("WIFI","no ssid");
            _log.set(WIFI_STATUS_ID,"no ssid");
            WiFi.mode(WIFI_STA);
            WiFi.begin(_ssid, _key);
            return;
        }
        case WL_SCAN_COMPLETED: 
            _log.info("WIFI","scan completed");
            _log.set(WIFI_STATUS_ID,"scan");
            return;
        case WL_CONNECTED: 
            _log.debug("WIFI","connected");
            _log.set(WIFI_STATUS_ID,"connected");
            return;
        case WL_CONNECT_FAILED:
        { 
            _log.warn("WIFI","connection failed");
            _log.set(WIFI_STATUS_ID,"link fail");
            WiFi.mode(WIFI_STA);
            WiFi.begin(_ssid, _key);
            return;
        }
        case WL_CONNECTION_LOST: 
        {
            _log.warn("WIFI","connection lost");
            _log.set(WIFI_STATUS_ID,"lost link");
            WiFi.mode(WIFI_STA);
            WiFi.begin(_ssid, _key);
            return;
        }
        case WL_DISCONNECTED: 
        {
            _log.warn("WIFI","disconnected");
            _log.set(WIFI_STATUS_ID,"disconn");
            WiFi.mode(WIFI_STA);
            WiFi.begin(_ssid, _key);
            return;
        }
    }
}

bool WifiMgr::isUp() {
    return WiFi.isConnected();
}