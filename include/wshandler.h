#pragma once
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include "xbeesettings.h"
#include "Arduino.h"

void procS(uint8_t *data, AsyncWebSocketClient *client)
{
    client->text('S' + getSettings());
}

void procU(uint8_t *data)
{
    String payload = String((char *)data);
    payload = payload.substring(payload.indexOf('{'), payload.indexOf('}') + 1);
    bool success = updateSettings(payload);
};

void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
    switch (type)
    {
    case WS_EVT_ERROR:
    {
    }
    case WS_EVT_PONG:
    {
    }
    case WS_EVT_CONNECT:
    {
    }
    case WS_EVT_DISCONNECT:
    {
    }
    case WS_EVT_DATA:
    {
        AwsFrameInfo *info = (AwsFrameInfo *)arg;
        if (!data)
        {
            break;
        }

        if (info->opcode == WS_TEXT)
        {
            switch (data[0])
            {
            case 'S':
                Serial.println("Settings requested");
                procS(data, client);
                break;
            case 'U':
                Serial.println("Setting Update Request");
                procU(data);
            }
        }
    }
    }
}