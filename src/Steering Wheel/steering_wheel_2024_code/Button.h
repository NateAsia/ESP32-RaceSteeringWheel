#ifndef BUTTON_H
#define BUTTON_H

#include "IInput.h"

class Button : public IInput {
public:
    Button(uint8_t pin, bool inverted);
    void setup() override;
    void update_state() override;
    bool state = false;


private:
    uint8_t pin;
    bool inverted;
};

#endif // BUTTON_H
