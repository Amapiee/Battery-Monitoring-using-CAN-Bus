#include "alert.h"
#include <Arduino.h>
#include "config.h"
namespace Alert {
void begin() { pinMode(LED_PIN, OUTPUT); pinMode(BUZZER_PIN, OUTPUT); digitalWrite(LED_PIN, LOW); digitalWrite(BUZZER_PIN, LOW); }
void update(bool active) { digitalWrite(LED_PIN, active ? HIGH : LOW); digitalWrite(BUZZER_PIN, active ? HIGH : LOW); }
}
