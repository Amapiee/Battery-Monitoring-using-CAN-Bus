#pragma once
#include <Arduino.h>

// CAN wiring to SN65HVD230 (adjust for your board)
constexpr gpio_num_t CAN_TX_PIN = GPIO_NUM_5;
constexpr gpio_num_t CAN_RX_PIN = GPIO_NUM_4;
constexpr uint32_t CAN_BITRATE = 500000;

// Sensor node pins
constexpr uint8_t TEMP_PIN = 15;       // DS18B20 data; use 4.7k pull-up to 3V3
constexpr uint8_t VOLTAGE_ADC_PIN = 34; // input-only ADC pin
constexpr float VOLTAGE_DIVIDER_RATIO = 2.0f; // Vinput = ADC voltage * ratio; calibrate
constexpr float ADC_REFERENCE_V = 3.3f;
constexpr uint16_t ADC_MAX = 4095;

// Monitoring node display and alert pins
constexpr uint8_t OLED_SDA_PIN = 21;
constexpr uint8_t OLED_SCL_PIN = 22;
constexpr uint8_t OLED_ADDRESS = 0x3C;
constexpr uint8_t LED_PIN = 2;
constexpr uint8_t BUZZER_PIN = 25; // external buzzer driver as appropriate

constexpr uint32_t SAMPLE_PERIOD_MS = 100;
constexpr uint32_t CAN_TIMEOUT_MS = 300;
constexpr float OVER_TEMP_C = 45.0f; // DEMO threshold; adjust for sensor/prototype
constexpr float WARNING_TEMP_C = 35.0f;
constexpr float OVER_VOLTAGE_V = 5.0f; // demo signal threshold, not pack voltage
constexpr float RAPID_RISE_C_PER_SEC = 0.5f; // heuristic, not thermal-runaway prediction
