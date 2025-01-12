#include "Button.h"
#include "esp_log.h"

Button::Button(uint8_t pin, bool inverted) : pin(pin), inverted(inverted) {}

void Button::setup()
{
    pinMode(pin, INPUT_PULLUP);
    ESP_LOGI(BASE_TAG, "Setup Button on pin %d", pin);
}

void Button::update_state()
{
    state = inverted ? !digitalRead(pin) : digitalRead(pin);
}
