#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "Wire.h"

namespace esphome {
namespace lwlp {

class LWLPSensor : public PollingComponent {
 public:
  sensor::Sensor *pressure_sensor = new sensor::Sensor();
  sensor::Sensor *temperature_sensor = new sensor::Sensor();

  // Polling every 1s
  LWLPSensor() : PollingComponent(1000) {}

  void setup() override {
    Wire.begin();
  }

  void update() override {
    Wire.beginTransmission(0x28);  // LWLP default I2C address
    Wire.endTransmission();
    Wire.requestFrom(0x28, 4);  // 2 bytes pressure + 2 bytes temp

    if (Wire.available() == 4) {
      uint16_t raw_pressure = (Wire.read() << 8) | Wire.read();
      uint16_t raw_temp = (Wire.read() << 8) | Wire.read();

      // --- Prevod podľa datasheetu LWLP ---
      float pressure_pa = ((raw_pressure - 1638) * 137) / (14745.0);  // Pa
      float temperature_c = (raw_temp / 65536.0) * 200.0 - 50.0;       // °C

      // Publikujeme do ESPHome senzora
      pressure_sensor->publish_state(pressure_pa);
      temperature_sensor->publish_state(temperature_c);
    }
  }
};

}  // namespace lwlp
}  // namespace esphome
