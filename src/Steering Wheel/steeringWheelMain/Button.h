#ifndef BUTTON_H
#define BUTTON_H

#include "IInput.h"

/**
 * @class Button
 * @brief Represents a physical button input.
 *
 * This class implements the IInput interface for a button,
 * handling setup and state updates for a digital input.
 */
class Button : public IInput {
public:
    /**
     * @brief Construct a new Button object.
     * 
     * @param pin The GPIO pin number to which the button is connected.
     * @param inverted If true, the button logic is inverted (LOW when pressed).
     */
    Button(uint8_t pin, bool inverted);

    /**
     * @brief Set up the button.
     * 
     * Initializes the GPIO pin for input.
     */
    void setup() override;

    /**
     * @brief Update the button state.
     * 
     * Reads the current state of the button from the GPIO pin.
     */
    void update_state() override;

    /**
     * @brief The current state of the button.
     * 
     * True if the button is pressed, false otherwise.
     */
    bool state = false;

private:
    /**
     * @brief The GPIO pin number to which the button is connected.
     */
    uint8_t pin;

    /**
     * @brief Indicates whether the button logic is inverted.
     * 
     * If true, a LOW signal means the button is pressed.
     */
    bool inverted;
};

#endif // BUTTON_H
