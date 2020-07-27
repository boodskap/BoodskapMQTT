#include <ESP8266WiFi.h>
#include <Ticker.h>
#include <BoodskapMQTT.h>
#include <MqttAdapter.h>

#define BAUD_RATE 115200
const char *WIFI_SSID = "your-wifi-ssid";
const char *WIFI_PASSWORD = "your-wifi-password";
const char *BOODSKAP_HOST = "your-boodskap-mqtt-host";
const uint16_t BOODSKAP_PORT = 1883;
const char *DOMAIN_KEY = "your-domain-key";
const char *API_KEY = "you-api-key";
const char *DEVICE_ID = "your-device-id";

BoodskapMQTT Boodskap;
WiFiClient wifiClient;
WiFiEventHandler wifiConnectHandler;
WiFiEventHandler wifiDisconnectHandler;
Ticker wifiReconnectTimer;
Ticker myProcessTimer;


void process();

void connectToWifi();

void onWifiConnect(const WiFiEventStationModeGotIP &event);

void onWifiDisconnect(const WiFiEventStationModeDisconnected &event);

class MqttCallback : public MqttAdapter
{

  virtual int onMessage(uint64_t mid, JsonObject &header, JsonObject &data)
  {
    Serial.println("Processing message...");
    String msgStr;
    serializeJson(data, Serial);
    Serial.println();
    return 1; //ack , return any other number for nack
  }

  virtual void onData(size_t len, byte *data)
  {
    Serial.println("Processing data...");
    Serial.println((const char*)data);
  }
};

MqttCallback listener;

void setup()
{

  Serial.begin(BAUD_RATE);

  delay(100);

  Serial.println();
  Serial.println();

  wifiConnectHandler = WiFi.onStationModeGotIP(onWifiConnect);
  wifiDisconnectHandler = WiFi.onStationModeDisconnected(onWifiDisconnect);

  Boodskap.setServer(BOODSKAP_HOST, BOODSKAP_PORT);
  Boodskap.setDomainKey(DOMAIN_KEY);
  Boodskap.setApiKey(API_KEY);
  Boodskap.setDeviceId(DEVICE_ID);
  Boodskap.setListener(&listener);
  Boodskap.setPrinter(Serial); //Disable this to stop printing in Serial console

  Serial.print("Domain ");
  Serial.println(Boodskap.domainKey());
  Serial.print("API ");
  Serial.println(Boodskap.apiKey());
  Serial.print("Device ID ");
  Serial.println(Boodskap.deviceId());
  Serial.println();

  connectToWifi();

  myProcessTimer.attach_ms_scheduled(15000, process); //schedule your process
}

void loop()
{
  Boodskap.loop();
}

uint16_t counter = 0;
/**
 * Entry point of your solution
 */
void process()
{

  if (!Boodskap.connected())
    return;

  StaticJsonDocument<MAX_PACKET_SIZE> msg;
  msg["counter"] = ++counter;
  msg["uptime"] = millis();

  Boodskap.send(102, msg);
}

void connectToWifi()
{

  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void onWifiConnect(const WiFiEventStationModeGotIP &event)
{
  Serial.println("Connected to Wi-Fi.");
  Boodskap.connect();
}

void onWifiDisconnect(const WiFiEventStationModeDisconnected &event)
{
  Serial.println("Disconnected from Wi-Fi.");
  wifiReconnectTimer.once(2, connectToWifi);
}
