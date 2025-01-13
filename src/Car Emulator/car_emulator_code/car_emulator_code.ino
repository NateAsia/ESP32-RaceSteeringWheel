#include "CarEmulator.h"

CarEmulator carEmulator;

void communication_setup() { 
  Serial.begin(115200); 
  for (int i=0; i<75; i++){
    ESP_LOGI("ESP",".");
    // Used to allow the existing logs
    // to make it to the serial monitor
  }
  ESP_LOGD("Setup", "ESP STARTING");
}

void setup() {
    communication_setup();
    carEmulator.setup();
    carEmulator.start();
}

void loop() {
    // Empty. Tasks are handled by FreeRTOS
    vTaskDelay(1000);
}