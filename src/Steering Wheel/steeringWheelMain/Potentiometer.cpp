#include "Potentiometer.h"
#include "esp_log.h"
#include "constants.h"

Potentiometer::Potentiometer(uint8_t pin) : pin(pin) {}

void Potentiometer::setup()
{
    pinMode(pin, INPUT);
    ESP_LOGD("Potentiometer", "Setup Potentiometer on pin %d", pin);
}

void Potentiometer::update_state()
{
    ESP_LOGV("Potentiometer", "reading %d", pin);
    state = analogReadMilliVolts(pin);
}