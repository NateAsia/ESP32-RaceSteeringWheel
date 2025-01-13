#include "RotaryButtonManager.h"
#include "esp_log.h"
#include "constants.h"

RotaryButtonManager::RotaryButtonManager(bool *left_btn_state, bool *right_btn_state)
{
    left_rty_btn.state = left_btn_state;
    left_rty_btn.num_of_modes = 4;
    left_rty_btn.eeprom_location = RTY_1_EE_LOCATION;

    right_rty_btn.state = right_btn_state;
    right_rty_btn.num_of_modes = 4;
    right_rty_btn.eeprom_location = RTY_2_EE_LOCATION;

    rotary_btn_list[0] = &left_rty_btn;
    rotary_btn_list[1] = &right_rty_btn;
}

void RotaryButtonManager::setup()
{
    setupEEPROM();
    ESP_LOGD("RotaryButtonManager", "setup complete");
}

void RotaryButtonManager::setupEEPROM()
{
    EEPROM.begin(EEPROM_SIZE);
    for (auto rotary_button : rotary_btn_list)
    {
        rotary_button->mode = EEPROM.readByte(rotary_button->eeprom_location);
    }
    ESP_LOGI("RotaryButtonManager", "EEPROM Setup Complete");
}

void RotaryButtonManager::update_state()
{
    ESP_LOGV("RotaryButtonManager", "updating");
    updateRotaryButtons();
}

void RotaryButtonManager::updateRotaryButtons()
{
    checkLastBtnPress();
    enterOrExistRotaryUpdateMode();
    checkForModeSwitches();
}

void RotaryButtonManager::checkForModeSwitches()
{
    // When in Rotary Update Mode, update external modes with a button press (rising edge)
    if (rty_mode_control.in_rty_update_mode)
    {
        for (auto rotary_button : rotary_btn_list)
        {
            // Start with debouncing the button
            if (rotary_button->last_state != *(rotary_button->state))
            {
                rotary_button->time_hold_start = millis();
                rotary_button->last_state = *(rotary_button->state);

                if (!*(rotary_button->state))
                {
                    rotary_button->disable = false;
                }
            }

            // Once Debounced, add to the mode counter if the button state is high (pressed) and not disabled (already pressed)
            if ((*(rotary_button->state)) & (!rotary_button->disable) & (millis() - rotary_button->time_hold_start > RTY_BTN_DEBOUNCE_LONG))
            {
                rotary_button->mode += *(rotary_button->state);
                rotary_button->disable = true;

                if (rotary_button->mode >= rotary_button->num_of_modes)
                {
                    rotary_button->mode = 0;
                }
            }
        }
    }
}

void RotaryButtonManager::enterOrExistRotaryUpdateMode()
{

    // Entering and Exiting Rotary Update Mode Using two buttons
    if (*(left_rty_btn.state) & *(right_rty_btn.state))
    {
        if (!rty_mode_control.dual_hold)
        {
            rty_mode_control.dual_hold = true;
            rty_mode_control.hold_start_time = millis();
        }
        else
        {
            if (!rty_mode_control.holding_too_long && millis() - rty_mode_control.hold_start_time >= ROTARY_WAIT_TIME)
            {
                toggleRotaryUpdateMode();
            }
            return; // if holding both, do not continue reading for mode switches
        }
    }
    else
    {
        rty_mode_control.dual_hold = false;
        rty_mode_control.holding_too_long = false;
    }

    // If we are in Rotary Update Mode and no buttons have been pressed for a while, exit Rotary Update Mode
    if (rty_mode_control.in_rty_update_mode & (millis() - rty_mode_control.last_btn_press > ROTARY_NO_INPUT_TIME))
    {
        toggleRotaryUpdateMode();
    }
}

void RotaryButtonManager::checkLastBtnPress()
{
    // Check if any rotary buttons are in pressed state
    for (auto& rotary_button : rotary_btn_list)
    {    
      if (*(rotary_button->state))
      {
          rty_mode_control.last_btn_press = millis();
      }
    }
}

void RotaryButtonManager::toggleRotaryUpdateMode()
{
    rty_mode_control.in_rty_update_mode = !rty_mode_control.in_rty_update_mode;
    rty_mode_control.holding_too_long = true;

    if (!rty_mode_control.in_rty_update_mode)
    { // When we exit rotary update mode, update the EEPROM
        for (auto& rotary_button : rotary_btn_list)
        {
            EEPROM.writeByte(rotary_button->eeprom_location, rotary_button->mode);
        }
        EEPROM.commit();
    }
    else
    {
        for (auto& rotary_button : rotary_btn_list)
        {
            rotary_button->disable = true;
        }
    }
}

bool RotaryButtonManager::in_rty_update_mode() const
{
    return rty_mode_control.in_rty_update_mode;
}

bool RotaryButtonManager::in_dual_hold() const
{
    return rty_mode_control.dual_hold;
}

uint8_t RotaryButtonManager::get_modes_status() const
{
    return (left_rty_btn.mode << 4) | (right_rty_btn.mode);
}

uint8_t RotaryButtonManager::get_screen_btns() const
{
    return ((*(left_rty_btn.state) & !rty_mode_control.in_rty_update_mode & !rty_mode_control.dual_hold) << 7) |
           ((*(right_rty_btn.state) & !rty_mode_control.in_rty_update_mode & !rty_mode_control.dual_hold) << 6);
}

bool RotaryButtonManager::in_left_special_btn_state() const
{
    return *(left_rty_btn.state) & !rty_mode_control.in_rty_update_mode & !rty_mode_control.dual_hold;
}

bool RotaryButtonManager::in_right_special_btn_state() const
{
    return *(right_rty_btn.state) & !rty_mode_control.in_rty_update_mode & !rty_mode_control.dual_hold;
}
