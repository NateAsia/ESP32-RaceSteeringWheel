#ifndef I_INPUT_H
#define I_INPUT_H

#include <Arduino.h>

/**
 * @class IInput
 * @brief Interface for input devices.
 *
 * This interface defines the basic operations that any input device
 * (e.g., buttons, potentiometers) should support.
 */
class IInput {
public:
    /**
     * @brief Set up the input device.
     */
    virtual void setup() = 0;

    /**
     * @brief Update the state of the input device.
     */
    virtual void update_state() = 0;

    /**
     * @brief Virtual destructor.
     *
     * Ensures proper cleanup of derived classes.
     */
    virtual ~IInput() = default;
};

#endif // I_INPUT_H
