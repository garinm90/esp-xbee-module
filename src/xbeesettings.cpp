#include "xbeesettings.h"
#include <XBee.h>
#include <ArduinoJson.h>
#include <Arduino.h>

String getSetting(uint8_t *Setting);

const int capacity = JSON_OBJECT_SIZE(10);
StaticJsonDocument<capacity> settings;

// serial high
uint8_t shCmd[] = {'S', 'H'};
// serial low
uint8_t slCmd[] = {'S', 'L'};

// Node ID
uint8_t idCmd[] = {'N', 'I'};
// Network ID
uint8_t netidCmd[] = {'I', 'D'};
// Network Channel
uint8_t chCmd[] = {'C', 'H'};

AtCommandRequest atRequest = AtCommandRequest();
AtCommandResponse atResponse = AtCommandResponse();
XBee xbee = XBee();
String serialHigh;

String getSettings()
{

    if (Serial2)
    {
        xbee.begin(Serial2);

        //Delay for xbee
        delay(100);

        settings["SH"] = getSetting(shCmd);
        settings["SL"] = getSetting(slCmd);
        settings["ID"] = getSetting(netidCmd);
        settings["NI"] = getSetting(idCmd);
        settings["CH"] = getSetting(chCmd);
        String response;
        serializeJson(settings, response);
        return response;
    }
}

bool updateSettings(uint8_t *Settings)
{
    const int jsonSize = JSON_OBJECT_SIZE(3);
    StaticJsonDocument<jsonSize> newSettings;
}

String getSetting(uint8_t *Setting)
{
    char SH[50];
    atRequest.setCommand(Setting);
    xbee.send(atRequest);

    if (xbee.readPacket(100))
    {
        xbee.getResponse()
            .getAtCommandResponse(atResponse);

        if (atResponse.getValueLength() > 0)
        {
            int index = 0;
            for (int i = 0; i < atResponse.getValueLength(); i++)
            {
                index += sprintf(&SH[index], "%02X", atResponse.getValue()[i]);
            }
            return String(SH);
        }
    }
    else
    {
        return String("error");
    }
}

bool xbeeStatus()
{
    if (Serial2)
    {
        xbee.begin(Serial2);
        String error = "error";
        String response = getSetting(shCmd);
        if (response.equals(error))
        {
            Serial.println(response);
            return false;
        }
        else
        {
            Serial.println(response);
            return true;
        }
    }
    else
    {
        Serial.println("Device not connected!");
        return false;
    }

    // atRequest.setCommand((uint8_t *)"TEST");
    // xbee.send(atRequest);

    // if (xbee.readPacket(100))
    // {
    //     if(at)
    //     Serial.println("Response Recieved");
    //     return true;
    // }
    // else
    // {
    //     return false;
    // }
    // Serial.println(response);
}
void setApiMode()
{
    String responses;
    Serial2.write("ATBD 7\r");
    Serial2.flush();
    responses = Serial2.readString();
    Serial.println(responses);
    Serial2.write("ATAP 2\r");
    Serial2.flush();
    responses = Serial2.readString();
    Serial.println(responses);
    Serial2.write("ATWR\r");
    Serial2.flush();
    responses = Serial2.readString();
    Serial.println(responses);
    Serial2.write("ATAC\r");
    Serial2.end();
    Serial2.begin(115200);
}