#include "Button.h"
#include "esp_log.h"
#include "constants.h"

Button::Button(uint8_t pin, bool inverted) : pin(pin), inverted(inverted) {}

void Button::setup()
{
    pinMode(pin, INPUT_PULLUP);
    ESP_LOGD("Button", "Setup Button on pin %d", pin);
    state = false;
}

void Button::update_state()
{
    ESP_LOGV("Button", "reading %d", pin);
    state = inverted ? !digitalRead(pin) : digitalRead(pin);
}
