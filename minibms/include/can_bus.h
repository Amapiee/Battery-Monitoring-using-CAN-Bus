#pragma once
#include <Arduino.h>
#include "driver/twai.h"

namespace CanBus {
bool begin();
bool send(uint32_t id, const uint8_t *data, uint8_t length);
bool receive(twai_message_t &message, uint32_t timeout_ms = 10);
}
