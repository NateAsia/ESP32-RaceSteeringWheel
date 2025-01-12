#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H
#include <stdint.h>

typedef struct ROTARY_BTN_T
{
    bool *state = nullptr;
    bool last_state = false;
    bool disable = true;
    long time_hold_start = 0;
    uint8_t mode = 0;
    uint8_t num_of_modes;
    uint8_t eeprom_location;

    // Default constructor
    ROTARY_BTN_T() = default;

    // Parameterized constructor
    ROTARY_BTN_T(bool* state, uint8_t num_of_modes, uint8_t eeprom_location)
        : state(state), num_of_modes(num_of_modes), eeprom_location(eeprom_location) {}
} ROTARY_BTN;

typedef struct ROTARY_MODE_CONTROL_T
{
    bool in_rty_update_mode = false; // If we are in Rotary Update Mode
    bool dual_hold = false;          // If we are holding both buttons
    bool holding_too_long = false;   // If we are holding both buttons for too long
    long hold_start_time = 0;        // Time when dual hold started
    long last_btn_press = 0;         // Last time any rotary button was pressed
} ROTARY_MODE_CONTROL;

#endif // STEERING_WHEEL_H