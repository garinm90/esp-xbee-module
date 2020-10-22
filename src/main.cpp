#include <Arduino.h>
#include <EEPROM.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>
#include <WiFiClient.h>
#include <XBee.h>
#include "xbeesettings.h"
#include "wshandler.h"

#define BUFFER_SIZE 15
#define ENTR_CMD_MODE "+++"
#define PAN_ID "ATID\r"

const char *tempssid = "Dennys2";
const char *temppass = "LEDlights2";

const uint32_t efuseid = ESP.getEfuseMac();

const char *password = "LEDLIGHTS1";
char ssid[BUFFER_SIZE];

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

void setup()
{
  snprintf(ssid, BUFFER_SIZE, "DSYNC-%X", efuseid);
  Serial.begin(115200);
  Serial2.begin(9600);
  Serial2.write("+++");
  Serial.println("Delay starts");
  vTaskDelay(1100 / portTICK_PERIOD_MS);
  Serial.println("Delay ends");
  String cmdModeResponse = Serial2.readString();

  if (cmdModeResponse.startsWith("OK"))
  {
    Serial.println("Command Mode entered");
    setApiMode();
  }
  else
  {
    // Serial.println("In API Mode");
    Serial2.end();
    Serial2.begin(115200);
    xbeeStatus();
  }

  WiFi.begin(tempssid, temppass);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println(WiFi.localIP());

  if (!SPIFFS.begin(true))
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  ws.onEvent(onWsEvent);
  server.addHandler(&ws);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/index.html", String(), false);
  });
  server.on("/css/bootstrap.min.css", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/css/bootstrap.min.css", "text/css");
  });
  server.on("/js/bootstrap.bundle.min.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/js/bootstrap.bundle.min.js", "text/javascript");
  });
  server.on("/js/jquery-3.3.1.min.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/js/jquery-3.3.1.min.js", "text/javascript");
  });

  // WiFi.softAP(ssid, password);
  // IPAddress IP = WiFi.softAPIP();
  // Serial.print("SSID: ");
  // Serial.println(ssid);
  // Serial.print("Password: ");
  // Serial.println(password);
  server.begin();
}

void loop()
{
}
