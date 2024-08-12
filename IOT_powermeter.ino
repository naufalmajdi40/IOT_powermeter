//cobagit
#if !( defined(ESP32) )
#error This code is designed for (ESP32 + W5500) to run on ESP32 platform! Please check your Tools->Board setting.
#endif
#include <Arduino.h>
#define _ASYNC_WEBSERVER_LOGLEVEL_       2
#include <SPI.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <AsyncWebServer_ESP32_W5500.h>
#include "SPIFFS.h"
#include <FS.h>
//#include "AsyncJson.h"
#define DEBUG_ETHERNET_WEBSERVER_PORT       Serial
int countdt[3] = {0, 0, 0};
//1 counting data masuk
//2 flag data count
//3counting data masuk yang sama
// Debug Level from 0 to 4
#define _ETHERNET_WEBSERVER_LOGLEVEL_       3
#define INT_GPIO            4
#define MISO_GPIO           19
#define MOSI_GPIO           23
#define SCK_GPIO            18
#define CS_GPIO             5
#include <WiFi.h>
float dtReal[3][100];
int modecfg ;
int restart = 0;
const String hexDigits = "0123456789ABCDEF";
long lastReconnectAttempt = 0;
byte perintah[3][6] ;
#define NUMBER_OF_MAC      20
byte mac[][NUMBER_OF_MAC] =
{
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x01 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x02 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x03 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x04 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x05 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x06 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x07 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x08 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x09 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x0A },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x0B },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x0C },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x0D },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x0E },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x0F },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x10 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x11 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x12 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x13 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x14 },
};
static const char alphanum[] = "0123456789"

                               "abcdefghijklmnopqrstuvwxyz"
                               "ABCDEFGHIJKLMNOPQRSTUVWXYZ";// For random generation of client ID.

WiFiClient   ethClient;
PubSubClient client(ethClient);

const char* ssid = "MONPWR";
const char* password = "MONPWR123#";

byte bufferDataModbus[200];
byte *ptr;
int ids, md, interval, wifiEn, type;
String kode, ip, nmsk, dns, gtw, mqtt_server, mqtt_name, mqtt_pass;

const char* http_username = "admin";
const char* http_password = "admin1234";
int flag = 0;
const long eventTime1 = 1000;
long eventTime2 = 5000;
const long eventTime3 = 5000;
unsigned long previousTime1 = 0;
unsigned long previousTime2 = 0;
unsigned long previousTime3 = 0;
void(* resetFunc) (void) = 0;
void callback(char* topic, byte* payload, unsigned int length) {
  // handle message arrived
}

boolean reconnect() {
  String mqttid = getrandomid();
  //Serial.println(Ethernet.localIP());
  //mqtt_name,mqtt_pass;
  //  Serial.println(mqtt_name);
  //Serial.println(mqtt_pass);

  if (client.connect(mqttid.c_str(), mqtt_name.c_str(), mqtt_pass.c_str())) {
    // Once connected, publish an announcement...
    //client.publish("outTopic", "hello world");
    client.subscribe("DASPWRCFG");
  }
  return client.connected();
}
DynamicJsonDocument doc(6000);
DynamicJsonDocument doc2(2048);
String processor(const String& var) {
  Serial.println(var);

  return "ok";
}

AsyncWebServer server(80);
void setup() {
  
  Serial.begin(115200);
  Serial.println();
  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  initConfig();
  initWebServer();
  ptr = bufferDataModbus;
  lastReconnectAttempt = 0;
  reconnect() ;
}

void loop()
{
  if (Serial.available() > 0)
  {
    readSerialData();
  }
  unsigned long currentTime = millis();

  if (currentTime - previousTime1 >= eventTime1) {
    sendModbus();
    if (countdt[0] != countdt[1]) {
      countdt[1] = countdt[0];
      countdt[2] = 0;
    }
    if (countdt[0] >= 100) {
      countdt[0] = 0;
     // countdt[2] = 0;
    }
    else {
      countdt[2]++;
    }
    if (countdt[2] > 5) {

      resetData();
      //Serial.println("reset sensor");
      saveSensorToJSON();
    }
    previousTime1 = currentTime;
  }
  while (Serial2.available() > 0)
  {
    byte b = Serial2.read();
    //Serial.print(b);
    *ptr++ = b;
    delay(2);
  }

  if (currentTime - previousTime2 >= eventTime2) {
    String dtSensor = "";
    //    for (int i = 0; i < 32; i++)
    //    {
    //      Serial.print(String(dtReal[2][i]));
    //      Serial.print("||");
    //    }
    String dtJson;
    serializeJson(doc,  dtJson);
    
    if (!client.connected()) {
      //Serial.pr
      Serial.println( dtJson);
      Serial.println("reconnect");
      reconnect();
    }
    else {
      String tpc="DASPOWER/DATA/DASPWR"+kode;
      client.publish(tpc.c_str(),  dtJson.c_str());
      Serial.println( dtJson);
      client.loop();
    }
    //Serial.println();
    previousTime2 = currentTime;
  }
  if (currentTime - previousTime3 >= eventTime3) {
    if (restart == 1) {
      restart++;
    }
    else if (restart == 2) {
      resetFunc();
    }
    previousTime3 = currentTime;

  }
}
