#include "Potentiometer.h"
#include "esp_log.h"

Potentiometer::Potentiometer(uint8_t pin) : pin(pin) {}

void Potentiometer::setup()
{
    pinMode(pin, INPUT);
    ESP_LOGI(BASE_TAG, "Setup Potentiometer on pin %d", pin);
}

uint16_t Potentiometer::update_state()
{
    state = analogReadMilliVolts(pin);
}