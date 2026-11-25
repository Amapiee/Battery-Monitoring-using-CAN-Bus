#pragma once
#include <Arduino.h>
namespace Display { bool begin(); void update(float tempC, float voltageV, bool tempValid, bool canTimeout, const String &faults); }
