#pragma once
#include <XBee.h>
#include <ArduinoJson.h>

String getSettings();
bool updateSettings(String);
void saveSetting();
void setApiMode();
bool xbeeStatus();