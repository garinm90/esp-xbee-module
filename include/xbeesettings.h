#pragma once
#include <XBee.h>
#include <ArduinoJson.h>

String getSettings();
void saveSetting();
void setApiMode();
bool xbeeStatus();