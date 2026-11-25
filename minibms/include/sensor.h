#pragma once
#include <Arduino.h>
namespace Sensor {
bool begin();
float readTemperatureC();
float readVoltageV();
}
