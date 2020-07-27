#ifndef _MQTT_ADAPTER_H_
#define _MQTT_ADAPTER_H_

#include <BoodskapGateway.h>
#include "MqttListener.h"

class MqttAdapter : public MqttListener
{

public:

    MqttAdapter()
    {

    }

    virtual int onMessage(uint64_t mid, JsonObject& header, JsonObject& data);

    virtual void onData(size_t len, char* data);

    virtual void onMqttMessage(char *topic, char *payload, size_t len);

    virtual void onMqttConnect(bool sessionPresent);

    virtual void onMqttDisconnect(AsyncMqttClientDisconnectReason reason);

    virtual void onMqttSubscribe(uint16_t packetId, uint8_t qos);

    virtual void onMqttUnsubscribe(uint16_t packetId);

    virtual void onMqttPublish(uint16_t packetId);


};

#endif //_MQTT_ADAPTER_H_
