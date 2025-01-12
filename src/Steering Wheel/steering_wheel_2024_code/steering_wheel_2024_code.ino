#include "esp_log.h"
#include <Arduino.h>
#include <stdio.h>
#include <stdlib.h>

#include "CANHandler.h"
#include "SteeringWheel.h"

CANHandler canHandler;
SteeringWheel steeringWheel(&canHandler);

void setup() {
  Serial.begin(115200);
  ESP_LOGI(BASE_TAG, "ESP-STARTING\n");

  canHandler.setup();
  steeringWheel.setup();
  steeringWheel.start();
}

void loop() {
  // Empty. Tasks are handled by FreeRTOS
}
