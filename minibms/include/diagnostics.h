#pragma once
#include <Arduino.h>
enum Fault : uint8_t { FAULT_NONE=0, FAULT_TEMP_SENSOR=1, FAULT_CAN_TIMEOUT=2, FAULT_OVERTEMP=3, FAULT_OVERVOLT=4, FAULT_RAPID_TEMP_RISE=5 };
namespace Diagnostics {
void set(Fault fault, bool active);
uint32_t activeMask();
String summary();
}
