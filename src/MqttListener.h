#ifndef _MQTT_LISTENER_H_
#define _MQTT_LISTENER_H_

#include <AsyncMqttClient.h>
#include <ArduinoJson.h>
#include "MessageListener.h"

class MqttListener : public MessageListener
{

public:

    virtual void onMqttMessage(char *topic, char *payload, size_t len) = 0;

    virtual void onMqttConnect(bool sessionPresent) = 0;

    virtual void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) = 0;

    virtual void onMqttSubscribe(uint16_t packetId, uint8_t qos);

    virtual void onMqttUnsubscribe(uint16_t packetId) = 0;

    virtual void onMqttPublish(uint16_t packetId) = 0;
};

#endif //_MQTT_LISTENER_H_
