
#include "esphome.h"
#include "esphome/core/log.h"

#define SENSOR_CNT 36
#define MAX_VALUES 50

class Ivt495Twin : public PollingComponent, public UARTDevice {
 public:
  // Store sensors returned by ESPHome
  std::vector<Sensor *> sensors;
  String inputString;
  float ivt[MAX_VALUES];
  int counter = 0;

  Ivt495Twin(UARTComponent *parent, const std::vector<Sensor *> &sensors_list)
      : UARTDevice(parent), sensors(sensors_list) {}

  void setup() override {
    set_update_interval(60000);  // Update every 60s
  }

  void update() override {
    // Nothing heavy here; handled in loop()
  }

  void loop() override {
    if (available() > 0) {
      fetchSerial();
      splitString();
      publishState();
    }
  }

  void fetchSerial() {
    while (available() > 0) {
      char inChar = read();
      if (inChar != ' ') inputString += inChar;
    }
  }

  void splitString() {
    counter = 0;
    int commaPosition;
    while ((commaPosition = inputString.indexOf(';')) >= 0 && counter < MAX_VALUES) {
      ivt[counter++] = inputString.substring(0, commaPosition).toFloat();
      inputString.remove(0, commaPosition + 1);
    }
    inputString.clear();
  }

  void publishState() {
    for (int i = 0; i < SENSOR_CNT && i < sensors.size(); i++) {
      if (sensors[i] == nullptr) continue;
      float value = ivt[i + 1];  // Adjust if data starts at index 1

      // Temperature sensors
      if (i < 8 || (i >= 19 && i <= 28)) {
        sensors[i]->publish_state(value / 10);
      }
      // Boolean states
      else if ((i >= 8 && i <= 18) || i == 32 || i == 35) {
        sensors[i]->publish_state(value != 0);
      }
      // Percent or integer values
      else {
        sensors[i]->publish_state((int)value);
      }
    }
  }
};
