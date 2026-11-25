#pragma once
#include <Arduino.h>

namespace Protocol {
constexpr uint32_t ID_VOLTAGE = 0x100;
constexpr uint32_t ID_TEMPERATURE = 0x101;
constexpr uint32_t ID_STATUS = 0x200;
constexpr uint8_t DLC = 4;

// All values are transmitted little-endian, scaled to integer units.
// Voltage frame: bytes 0..1 = millivolts (uint16), bytes 2..3 reserved.
// Temperature frame: bytes 0..1 = centi-degrees C (int16), bytes 2..3 reserved.
void encodeU16(uint16_t value, uint8_t out[2]);
uint16_t decodeU16(const uint8_t in[2]);
void encodeI16(int16_t value, uint8_t out[2]);
int16_t decodeI16(const uint8_t in[2]);
}
