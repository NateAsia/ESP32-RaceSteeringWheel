#ifndef POTENTIOMETER_H
#define POTENTIOMETER_H

#include "IInput.h"

class Potentiometer : public IInput {
public:
    Potentiometer(uint8_t pin);
    void setup() override;
    uint16_t update_state() override;
    uint16_t state = 0;

private:
    uint8_t pin;
};

#endif // POTENTIOMETER_H
