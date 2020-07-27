#include <ArduinoJson.h>
#include "BoodskapMQTT.h"
#include "MqttAdapter.h"

BoodskapMQTT *THIS;

void onMqttConnect(bool sessionPresent);
void onMqttDisconnect(AsyncMqttClientDisconnectReason reason);
void onMqttSubscribe(uint16_t packetId, uint8_t qos);
void onMqttUnsubscribe(uint16_t packetId);
void onMqttMessage(char *topic, char *payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total);
void onMqttPublish(uint16_t packetId);

BoodskapMQTT::BoodskapMQTT() : BoodskapGateway(), _listener(NULL)
{
    ::THIS = this;
}

void BoodskapMQTT::setListener(MqttListener *listener)
{
    this->_listener = listener;
    
    BoodskapGateway::setListener(listener);
}

MqttListener *BoodskapMQTT::listener()
{
    return this->_listener;
}

AsyncMqttClient *BoodskapMQTT::mqtt()
{
    return &this->_mqtt;
}

bool BoodskapMQTT::connected()
{
    return _mqtt.connected();
}

bool BoodskapMQTT::connect()
{

    if (connected())
        return true;

    String prefix = "DEV_";
    String clientId = prefix + String(_deviceId);
    String userId = prefix + String(_domainKey);

    strcpy(_clientId, clientId.c_str());
    strcpy(_userId, userId.c_str());

    print("Connecting to MQTT server ");
    print(_ip);
    printf(":%d\n", _port);


    print("UserID: ");
    println(_userId);

    print("Passwd: ");
    println(_apiKey);

    print("ClientID: ");
    println(_clientId);

    _mqtt.setServer(_ip, _port);
    _mqtt.setCredentials(_userId, _apiKey);
    _mqtt.setClientId(_clientId);
    _mqtt.setCleanSession(true);
    _mqtt.setKeepAlive(15);
    _mqtt.onConnect(onMqttConnect);
    _mqtt.onDisconnect(onMqttDisconnect);
    _mqtt.onSubscribe(onMqttSubscribe);
    _mqtt.onUnsubscribe(onMqttUnsubscribe);
    _mqtt.onMessage(onMqttMessage);
    _mqtt.onPublish(onMqttPublish);

    _mqtt.connect();

    return connected();
}

bool BoodskapMQTT::disconnect()
{
    _mqtt.disconnect();

    return true;
}

void BoodskapMQTT::loop()
{

    //if (!connect())
      // return;

    //_mqtt.loop();
}

bool BoodskapMQTT::send(uint64_t mid, JsonDocument &data)
{

    char topic[PUBLISH_TOPIC_SIZE];
    snprintf(topic, PUBLISH_TOPIC_SIZE, "/%s/device/%s/msgs/%s/%s/%s", _domainKey, _deviceId, _deviceModel, _fwVersion, uint64_to_string(mid));
    String output;
    serializeJson(data, output);
    printf("SENDING: topic:%s, msg:%s\n", topic, output.c_str());
    return _mqtt.publish(topic, 2, false, output.c_str());
}

bool BoodskapMQTT::send(const char *type, const char *data)
{

    char topic[PUBLISH_TOPIC_SIZE];
    snprintf(topic, PUBLISH_TOPIC_SIZE, "/%s/device/%s/msgs/bin/%s/%s/%s", _domainKey, _deviceId, _deviceModel, _fwVersion, type);
    return _mqtt.publish(topic, 2, false, data);
}

const char *BoodskapMQTT::trigger(const char *rule, JsonDocument &args)
{
    return 0;
}

const char *BoodskapMQTT::insert(uint64_t rid, JsonDocument &data)
{
    return 0;
}

const char *BoodskapMQTT::insert(uint64_t rid, const char *id, JsonDocument &data)
{
    return 0;
}

void onMqttConnect(bool sessionPresent)
{

    THIS->println("Connected, subscribing for incoming commands...");

    char topic[SUBSCRIBE_TOPIC_SIZE];
    snprintf(topic, SUBSCRIBE_TOPIC_SIZE, "/%s/device/%s/cmds", THIS->domainKey(), THIS->deviceId());

    THIS->mqtt()->subscribe(topic, 0);

    if (THIS->listener() == NULL)
        return;

    THIS->listener()->onMqttConnect(sessionPresent);
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason)
{

    DEBUG_PRINTF(THIS->printer(), "Disconnected from MQTT, Reason:%d\n", static_cast<uint8_t>(reason));

    if (THIS->listener() == NULL)
        return;

    THIS->listener()->onMqttDisconnect(reason);
}

void onMqttSubscribe(uint16_t packetId, uint8_t qos)
{

    THIS->println("Subscription acknowledged.");
    if (THIS->listener() == NULL)
        return;

    THIS->listener()->onMqttSubscribe(packetId, qos);
}

void onMqttUnsubscribe(uint16_t packetId)
{
    THIS->println("Unsubscribe acknowledged.");

    if (THIS->listener() == NULL)
        return;

    THIS->listener()->onMqttUnsubscribe(packetId);
}

void onMqttMessage(char *topic, char *payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total)
{
    payload[len] = '\0';
    THIS->print("Message received, topic: ");
    THIS->println(topic);

    if (THIS->listener() == NULL)
        return;

    THIS->listener()->onMqttMessage(topic, payload, len);

    THIS->processMessage((byte*)payload, len);
}

void onMqttPublish(uint16_t packetId)
{

    THIS->println("Message published.");

    if (THIS->listener() == NULL)
        return;

    THIS->listener()->onMqttPublish(packetId);
}
