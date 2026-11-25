#include "sensor.h"
#include "config.h"
#include <OneWire.h>
#include <DallasTemperature.h>
namespace {
OneWire oneWire(TEMP_PIN);
DallasTemperature ds(&oneWire);
}
namespace Sensor {
bool begin() { ds.begin(); ds.setResolution(12); return ds.getDeviceCount() > 0; }
float readTemperatureC() { ds.requestTemperatures(); return ds.getTempCByIndex(0); }
float readVoltageV() {
  // ESP32 ADC calibration is approximate. Validate and calibrate against a DMM.
  const uint16_t raw = analogRead(VOLTAGE_ADC_PIN);
  const float adcV = (float(raw) / ADC_MAX) * ADC_REFERENCE_V;
  return adcV * VOLTAGE_DIVIDER_RATIO;
}
}
