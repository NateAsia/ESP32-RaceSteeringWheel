#ifndef I_INPUT_H
#define I_INPUT_H

#include <Arduino.h>

class IInput {
public:
    virtual void setup() = 0;
    virtual void update_state() = 0;
    virtual ~IInput() = default;
};

#endif // I_INPUT_H
