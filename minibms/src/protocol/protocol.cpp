#include "protocol.h"
namespace Protocol {
void encodeU16(uint16_t v, uint8_t o[2]) { o[0] = uint8_t(v & 0xFF); o[1] = uint8_t(v >> 8); }
uint16_t decodeU16(const uint8_t i[2]) { return uint16_t(i[0]) | (uint16_t(i[1]) << 8); }
void encodeI16(int16_t v, uint8_t o[2]) { encodeU16(static_cast<uint16_t>(v), o); }
int16_t decodeI16(const uint8_t i[2]) { return static_cast<int16_t>(decodeU16(i)); }
}
