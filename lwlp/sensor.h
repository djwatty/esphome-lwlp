#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "Wire.h"

namespace esphome {
namespace lwlp {

class LWLPSensor : public PollingComponent, public sensor::Sensor {
 public:
  LWLPSensor() : PollingComponent(1000) {}

  void setup() override {
    Wire.begin();
  }

  void update() override {
    Wire.beginTransmission(0x28);  // LWLP default I2C address
    Wire.endTransmission();
    Wire.requestFrom(0x28, 2);

    if (Wire.available() == 2) {
      uint16_t raw = (Wire.read() << 8) | Wire.read();
      float pressure = raw * 1.0;  // ≈ RAW conversion (doladíme neskôr podľa datasheetu)
      publish_state(pressure);
    }
  }
};

}  // namespace lwlp
}  // namespace esphome

