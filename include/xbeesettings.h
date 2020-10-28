#pragma once
#include <XBee.h>
#include <ArduinoJson.h>

String getSettings();
bool updateSettings(String);
void sendAtCommand(const char *command, const char *parameter);
void saveSetting();
void setApiMode();
bool xbeeStatus();