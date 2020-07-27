#include "MqttAdapter.h"

void MqttAdapter::onMqttMessage(char *topic, char *payload, size_t len){

}

int MqttAdapter::onMessage(uint64_t mid, JsonObject& header, JsonObject& data){
    return 1;
}

void MqttAdapter::onData(size_t len, char* data){

}

void MqttAdapter::onMqttConnect(bool sessionPresent){

}

void MqttAdapter::onMqttDisconnect(AsyncMqttClientDisconnectReason reason){

}

void MqttAdapter::onMqttSubscribe(uint16_t packetId, uint8_t qos){

}

void MqttAdapter::onMqttUnsubscribe(uint16_t packetId){

}

void MqttAdapter::onMqttPublish(uint16_t packetId){

}
