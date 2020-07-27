#ifndef _BOODSKAP_MQTT_H_
#define _BOODSKAP_MQTT_H_

#include <BoodskapGateway.h>
#include <ArduinoJson.h>
#include <Client.h>
#include <AsyncMqttClient.h>
#include "MqttListener.h"

#define PUBLISH_TOPIC_SIZE (DEVICE_ID_SIZE+DEVICE_MODEL_SIZE+FW_VERSION_SIZE+60)
#define SUBSCRIBE_TOPIC_SIZE (DOMAIN_KEY_SIZE+DEVICE_ID_SIZE+40)

class BoodskapMQTT : public BoodskapGateway {

    public:

    BoodskapMQTT();

    void setListener(MqttListener* listener);

    MqttListener* listener();

    AsyncMqttClient* mqtt();

    virtual bool connect();

    virtual bool disconnect();

    virtual bool connected();

    virtual void loop();

    virtual bool send(uint64_t mid, JsonDocument& data);

    virtual bool send(const char* type, const char* data);

    virtual const char* trigger(const char* rule, JsonDocument& args);

    virtual const char* insert(uint64_t rid, JsonDocument& data);

    virtual const char* insert(uint64_t rid, const char* id, JsonDocument& data);

    private:
        AsyncMqttClient _mqtt;
        MqttListener* _listener;
        char _userId[DOMAIN_KEY_SIZE+4];
        char _clientId[DEVICE_ID_SIZE+4];
};

#endif // _BOODSKAP_MQTT_H_
