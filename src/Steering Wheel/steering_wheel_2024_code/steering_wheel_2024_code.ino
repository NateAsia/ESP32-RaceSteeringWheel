#include "esp_log.h"
#include <Arduino.h>
#include <stdio.h>
#include <stdlib.h>

#include "CANHandler.h"
#include "SteeringWheel.h"

CANHandler canHandler;
SteeringWheel steeringWheel(&canHandler);

void communication_setup() { 
  Serial.begin(115200); 
  for (int i=0; i<75; i++){
    ESP_LOGI("ESP",".");
    // Used to allow the existing logs
    // to make it to the serial monitor
  }
}

void setup() {
  communication_setup();
  ESP_LOGI("setup", "ESP-STARTING\n");
  canHandler.setup();
  steeringWheel.setup();
  steeringWheel.start();
}

void loop() {
  // Empty. Tasks are handled by FreeRTOS
  vTaskDelay(1000);
}
