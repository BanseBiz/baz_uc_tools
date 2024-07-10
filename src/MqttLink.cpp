#include <MqttLink.h>
#include <functional>

#define MQTT_CONN_STATE_TOPIC "STATUS"

std::map<std::string,MqttLink::sub_func_t> MqttLink::subTopics;

MqttLink::MqttLink(mqtt_config_t& config, Client& client)
    : _config(config), _client(client)
{

}

int MqttLink::connect() {
    return connect(_config.server, _config.port);
}

int MqttLink::connect(const char* server, uint16_t port) {
    _client.setServer(server,port);
    _client.connect(_config.client_id);
    _client.connect(_config.client_id,_config.user,_config.pass,_config.willTopic,
                    _config.willQos,_config.willRetain,_config.willMessage);
    if (_client.connected()) {
        char buffer[64];
        snprintf(buffer,64,"%s joins to publish on %s",_config.client_id,_config.pupTopic);
        _client.publish(MQTT_CONN_STATE_TOPIC,buffer,strnlen(buffer,64));
        _client.setCallback(MqttLink::callback);
        for (auto const& topic : subTopics) {
            _client.subscribe(topic.first.c_str());
        }
        return true;
    }
    return false;
}

int MqttLink::connect(IPAddress ip, uint16_t port) {
    _client.setServer(ip,port);
    _client.connect(_config.client_id,_config.user,_config.pass,_config.willTopic,
                    _config.willQos,_config.willRetain,_config.willMessage);
    if (_client.connected()) {
        char buffer[64];
        snprintf(buffer,64,"%s joins to publish on %s",_config.client_id,_config.pupTopic);
        _client.publish(MQTT_CONN_STATE_TOPIC,buffer,strnlen(buffer,64));
        _client.setCallback(MqttLink::callback);
        for (auto const& topic : subTopics) {
            _client.subscribe(topic.first.c_str());
        }
        return true;
    }
    return false;
}

size_t MqttLink::write(uint8_t byte) {
    return _client.publish(_config.pupTopic, &byte, 1);
}

size_t MqttLink::write(const uint8_t* msg, size_t size) {
    return _client.publish(_config.pupTopic, msg, size);
}

int MqttLink::available() {
    return 0;
}

int MqttLink::read() {
    return 0;
}

int MqttLink::read(uint8_t*, size_t) {
    return 0;
}

int MqttLink::peek() {
    return 0;
}

void MqttLink::flush() {
    
}

void MqttLink::stop() {
    _client.disconnect();
}

uint8_t MqttLink::connected() {
    return _client.connected();
}

MqttLink::operator bool() {
    return _client.connected();
}

void MqttLink::subscibe(std::string topic, sub_func_t func) {
    _client.subscribe(topic.c_str());
    subTopics[topic] = func;
}

void MqttLink::callback(char* c_topic, byte* message, unsigned int length) {
    Serial.print("received msg");
    std::string topic = std::string(c_topic);
    std::map<std::string,sub_func_t>::iterator it = subTopics.find(topic);
    if (it == subTopics.end()) {
        return;
    }
    subTopics[topic](message,length);
}