#include "can_bus.h"
#include "config.h"
namespace CanBus {
bool begin() {
  twai_general_config_t g = TWAI_GENERAL_CONFIG_DEFAULT(CAN_TX_PIN, CAN_RX_PIN, TWAI_MODE_NORMAL);
  g.tx_queue_len = 10; g.rx_queue_len = 10;
  twai_timing_config_t t = TWAI_TIMING_CONFIG_500KBITS();
  twai_filter_config_t f = TWAI_FILTER_CONFIG_ACCEPT_ALL();
  if (twai_driver_install(&g, &t, &f) != ESP_OK) return false;
  return twai_start() == ESP_OK;
}
bool send(uint32_t id, const uint8_t *data, uint8_t length) {
  if (length > 8) return false;
  twai_message_t m{}; m.identifier = id; m.extd = 0; m.rtr = 0; m.data_length_code = length;
  for (uint8_t i = 0; i < length; ++i) m.data[i] = data[i];
  return twai_transmit(&m, pdMS_TO_TICKS(20)) == ESP_OK;
}
bool receive(twai_message_t &m, uint32_t timeout_ms) { return twai_receive(&m, pdMS_TO_TICKS(timeout_ms)) == ESP_OK; }
}
