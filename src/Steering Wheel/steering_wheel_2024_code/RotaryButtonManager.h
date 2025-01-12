#ifndef ROTARY_BUTTON_MANAGER_H
#define ROTARY_BUTTON_MANAGER_H

#include "IInput.h"
#include "data_strcutures.h"
#include <EEPROM.h>
#include "constants.h"

class RotaryButtonManager : public IInput {
public:
    RotaryButtonManager(bool* left_btn_state, bool* right_btn_state);  // Modified constructor
    void setup() override;
    void update_state() override;
    bool get_in_rty_update_mode() const;
    bool in_dual_hold() const;
    uint8_t get_modes_status() const;
    uint8_t get_screen_btns() const;
    bool in_left_special_btn_state() const;
    bool in_right_special_btn_state() const;

private:
    ROTARY_BTN left_rty_btn;
    ROTARY_BTN right_rty_btn;
    ROTARY_BTN *rotary_btn_list[ROTARY_BTN_LIST_SIZE];
    ROTARY_MODE_CONTROL rty_mode_control;

    void setupEEPROM();
    void updateRotaryButtons();
    void checkLastBtnPress();
    void updateCustomStates();
    void enterOrExistRotaryUpdateMode();
    void checkForModeSwitches();
    void toggleRotaryUpdateMode();
};

#endif // ROTARY_BUTTON_MANAGER_H
