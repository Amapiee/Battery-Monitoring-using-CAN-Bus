#include "display.h"
#include "config.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
namespace { Adafruit_SSD1306 oled(128, 64, &Wire, -1); }
namespace Display {
bool begin() { Wire.begin(OLED_SDA_PIN, OLED_SCL_PIN); if (!oled.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) return false; oled.clearDisplay(); oled.setTextColor(SSD1306_WHITE); oled.display(); return true; }
void update(float t, float v, bool valid, bool timeout, const String &faults) {
  oled.clearDisplay(); oled.setTextSize(1); oled.setCursor(0,0); oled.println("MINI BMS / MONITOR");
  oled.setCursor(0,14); oled.print("Temp: "); if(valid) { oled.print(t,1); oled.println(" C"); } else oled.println("SENSOR ERR");
  oled.setCursor(0,26); oled.print("Voltage: "); if(!timeout) { oled.print(v,2); oled.println(" V"); } else oled.println("--");
  oled.setCursor(0,38); oled.println(timeout ? "CAN: TIMEOUT" : "CAN: ONLINE");
  oled.setCursor(0,50); oled.print(faults.substring(0,20)); oled.display();
}
}
