#pragma once

#include <Client.h>
#include <PubSubClient.h>
#include <map>

struct mqtt_config_t
{
    const char* client_id;
    const char* server;
    uint16_t port;
    const char* user;
    const char* pass;
    const char* willTopic;
    uint8_t willQos;
    boolean willRetain;
    const char* willMessage;
    const char* pupTopic;
};

class MqttLink : public Client
{
    
public:
    typedef void (*sub_func_t)(uint8_t*,unsigned int);
    MqttLink(mqtt_config_t&, Client&);
    int connect();
    int connect(const char*, uint16_t) override;
    int connect(IPAddress ip, uint16_t port) override;
    size_t write(uint8_t) override;
    size_t write(const uint8_t *buf, size_t size) override;
    int available() override;
    int read() override;
    int read(uint8_t*, size_t) override;
    int peek() override;
    void flush() override;
    void stop() override;
    uint8_t connected() override;
    operator bool() override;
    void subscibe(std::string, sub_func_t);

    private:
    mqtt_config_t& _config;
    PubSubClient _client;
    static std::map<std::string,sub_func_t> subTopics;

    static void callback(char*, uint8_t*, unsigned int);
};