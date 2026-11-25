# Mini BMS (2-node ESP32 CAN prototype)

Educational low-voltage prototype: sensor ESP32 sends voltage and DS18B20 temperature over Classical CAN to a monitoring ESP32. Monitoring node evaluates basic faults and drives OLED/LED/buzzer.

## Build/upload
- PlatformIO project in VS Code.
- Upload to Sensor Node: `pio run -e sensor_node -t upload`
- Upload to Monitor Node: `pio run -e monitor_node -t upload`
- Serial monitor: `pio device monitor -b 115200`

## Wiring (adjust pins in `include/config.h`)
### Both ESP32 nodes + CAN transceiver
- ESP32 TX GPIO5 -> transceiver TXD (often called TX or DI)
- ESP32 RX GPIO4 <- transceiver RXD (often called RX or RO)
- ESP32 3V3 -> transceiver VCC (confirm module is 3.3V-compatible)
- ESP32 GND -> transceiver GND
- Transceiver CANH to CANH, CANL to CANL
- 120-ohm termination only at each physical end of the bus (two ends total)

### Sensor node
- DS18B20 data -> GPIO15; 4.7k pull-up from data to 3V3; sensor VDD -> 3V3, GND -> GND
- Voltage-divider output -> GPIO34. Never exceed ESP32 ADC input limits. Use a low-voltage/current-limited bench source for demo and calibrate against a DMM.

### Monitor node
- OLED SDA -> GPIO21, SCL -> GPIO22, VCC/GND according to module
- LED output GPIO2 (use series resistor if external LED)
- Buzzer GPIO25: use a transistor/driver as needed; do not drive a high-current buzzer directly from GPIO

## Protocol
- 500 kbps, standard 11-bit CAN, DLC 4
- ID 0x100: voltage in millivolts, uint16 little-endian bytes 0-1
- ID 0x101: temperature in centi-degrees C, int16 little-endian bytes 0-1; 0x7FFF = invalid
- IDs are a custom protocol for this project, not Orion BMS protocol.

## Safety / scope
This is a demo monitoring system, not a certified BMS and not a replacement for cell monitoring/protection. The voltage threshold is a demo signal threshold, not a lithium-cell or pack safety limit. Do not connect to a live high-voltage battery pack or intentionally overheat/overcharge cells. Validate using simulated signals or safe, current-limited low-voltage sources.

## Modules
- `sensor`: sensor readout
- `can_bus`: TWAI driver wrapper
- `protocol`: encoding/decoding
- `diagnostics`: fault flags
- `monitor`: application logic (currently in main.cpp, should be extracted as next refactor)
- `display`, `alert`: output modules
