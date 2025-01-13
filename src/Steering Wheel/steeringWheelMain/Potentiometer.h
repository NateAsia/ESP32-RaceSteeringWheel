#ifndef POTENTIOMETER_H
#define POTENTIOMETER_H

#include "IInput.h"

/**
 * @class Potentiometer
 * @brief Represents a potentiometer input.
 *
 * This class implements the IInput interface for a potentiometer,
 * handling setup and state updates for an analog input.
 */
class Potentiometer : public IInput {
public:
    /**
     * @brief Construct a new Potentiometer object.
     * 
     * @param pin The analog pin number to which the potentiometer is connected.
     */
    Potentiometer(uint8_t pin);

    /**
     * @brief Set up the potentiometer.
     * 
     * Initializes the analog pin for input.
     */
    void setup() override;

    /**
     * @brief Update the potentiometer state.
     * 
     * Reads the current value of the potentiometer from the analog pin.
     */
    void update_state() override;

    /**
     * @brief The current state of the potentiometer.
     * 
     * Represents the current analog reading from the potentiometer.
     */
    uint16_t state = 0;

private:
    /**
     * @brief The analog pin number to which the potentiometer is connected.
     */
    uint8_t pin;
};

#endif // POTENTIOMETER_H
