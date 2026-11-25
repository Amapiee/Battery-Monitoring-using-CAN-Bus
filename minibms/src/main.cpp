#include <Arduino.h>
#include "config.h"
#include "can_bus.h"
#include "protocol.h"
#include "sensor.h"
#include "diagnostics.h"
#include "display.h"
#include "alert.h"

#ifdef NODE_SENSOR

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);
  pinMode(VOLTAGE_ADC_PIN, INPUT);
  const bool sensorFound = Sensor::begin();
  Serial.println(sensorFound ? "Temperature sensor detected" : "WARNING: no DS18B20 detected");
  if (!CanBus::begin()) { Serial.println("CAN init failed"); while (true) delay(1000); }
  Serial.println("Sensor node started");
}
void loop() {
  static uint32_t last = 0;
  if (millis() - last >= SAMPLE_PERIOD_MS) {
    last = millis();
    const float temp = Sensor::readTemperatureC();
    const float volt = Sensor::readVoltageV();
    uint8_t payload[4] = {0};
    // Voltage: millivolts, uint16. Clip to representable range.
    const uint16_t mv = (uint16_t)constrain((int)(volt * 1000.0f), 0, 65535);
    Protocol::encodeU16(mv, payload);
    CanBus::send(Protocol::ID_VOLTAGE, payload, Protocol::DLC);
    // Temperature: centi-degrees C. 0x7FFF signals invalid sensor.
    int16_t centi = 0x7FFF;
    if (temp > -55.0f && temp < 125.0f && temp != 85.0f) centi = (int16_t)(temp * 100.0f);
    Protocol::encodeI16(centi, payload);
    CanBus::send(Protocol::ID_TEMPERATURE, payload, Protocol::DLC);
    Serial.printf("TX | voltage %.3f V | temperature %.2f C\n", volt, temp);
  }
}

#elif defined(NODE_MONITOR)

namespace {
float voltage = 0.0f, temperature = 0.0f, previousTemp = 0.0f;
bool voltageSeen = false, tempSeen = false, tempValid = false;
uint32_t lastVoltage = 0, lastTemp = 0, previousTempTime = 0;
float riseRate = 0.0f;
void processFrame(const twai_message_t &m) {
  if (m.data_length_code < 2) return;
  if (m.identifier == Protocol::ID_VOLTAGE) {
    voltage = Protocol::decodeU16(m.data) / 1000.0f; voltageSeen = true; lastVoltage = millis();
  } else if (m.identifier == Protocol::ID_TEMPERATURE) {
    const int16_t centi = Protocol::decodeI16(m.data);
    lastTemp = millis(); tempSeen = true;
    tempValid = centi != 0x7FFF;
    if (tempValid) {
      const float newTemp = centi / 100.0f;
      const uint32_t now = millis();
      if (previousTempTime && now > previousTempTime) riseRate = (newTemp - previousTemp) / ((now - previousTempTime) / 1000.0f);
      previousTemp = temperature; // kept for basic continuity; below, previousTemp is corrected
      previousTemp = newTemp; previousTempTime = now;
      temperature = newTemp;
    }
  }
}
}
void setup() {
  Serial.begin(115200);
  Alert::begin();
  Display::begin();
  if (!CanBus::begin()) { Serial.println("CAN init failed"); while (true) delay(1000); }
  Serial.println("Monitor node started");
}
void loop() {
  twai_message_t frame{};
  while (CanBus::receive(frame, 5)) processFrame(frame);
  const uint32_t now = millis();
  const bool canTimeout = (!voltageSeen || now - lastVoltage > CAN_TIMEOUT_MS) || (!tempSeen || now - lastTemp > CAN_TIMEOUT_MS);
  Diagnostics::set(FAULT_CAN_TIMEOUT, canTimeout);
  Diagnostics::set(FAULT_TEMP_SENSOR, tempSeen && !tempValid);
  Diagnostics::set(FAULT_OVERTEMP, tempValid && temperature >= OVER_TEMP_C);
  Diagnostics::set(FAULT_OVERVOLT, voltageSeen && voltage >= OVER_VOLTAGE_V);
  Diagnostics::set(FAULT_RAPID_TEMP_RISE, tempValid && riseRate >= RAPID_RISE_C_PER_SEC);
  const bool anyFault = Diagnostics::activeMask() != 0;
  Alert::update(anyFault);
  Display::update(temperature, voltage, tempValid, canTimeout, Diagnostics::summary());
  Serial.printf("RX | V=%.2f V | T=%.2f C | dT/dt=%.2f C/s | %s\n", voltage, temperature, riseRate, Diagnostics::summary().c_str());
  delay(100);
}
#else
#error "Select NODE_SENSOR or NODE_MONITOR in platformio.ini"
#endif
