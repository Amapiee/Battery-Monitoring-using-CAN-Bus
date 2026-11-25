#include "diagnostics.h"
namespace { uint32_t mask = 0; }
namespace Diagnostics {
void set(Fault f, bool active) { if (f == FAULT_NONE) return; const uint32_t bit = 1UL << f; if (active) mask |= bit; else mask &= ~bit; }
uint32_t activeMask() { return mask; }
String summary() {
  if (!mask) return "NORMAL";
  String s;
  if (mask & (1UL<<FAULT_TEMP_SENSOR)) s += "TEMP SENSOR; ";
  if (mask & (1UL<<FAULT_CAN_TIMEOUT)) s += "CAN TIMEOUT; ";
  if (mask & (1UL<<FAULT_OVERTEMP)) s += "OVERTEMP; ";
  if (mask & (1UL<<FAULT_OVERVOLT)) s += "OVERVOLT; ";
  if (mask & (1UL<<FAULT_RAPID_TEMP_RISE)) s += "TEMP RISE; ";
  return s;
}
}
