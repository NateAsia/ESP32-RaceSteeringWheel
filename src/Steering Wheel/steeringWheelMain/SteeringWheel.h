#ifndef STEERING_WHEEL_H
#define STEERING_WHEEL_H

#include "ISteeringWheel.h"
#include "ICAN.h"
#include "IInput.h"
#include "Button.h"
#include "Potentiometer.h"
#include "RotaryButtonManager.h"
#include "constants.h"
#include <vector>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

/**
 * @class SteeringWheel
 * @brief Implements the ISteeringWheel interface.
 *
 * This class provides a concrete implementation of the steering wheel
 * functionality, managing inputs, state, and CAN communication.
 */
class SteeringWheel : public ISteeringWheel
{
public:
    /**
     * @brief Construct a new Steering Wheel object.
     * 
     * @param can Pointer to the CAN interface.
     */
    SteeringWheel(ICAN *can);

    /**
     * @brief Set up the steering wheel.
     * 
     * Initializes all inputs and prepares the steering wheel for operation.
     */
    void setup() override;

    /**
     * @brief Start the steering wheel operations.
     * 
     * Begins the main operational loop, including input processing and CAN transmission.
     */
    void start() override;

private:
    // Object variables
    ICAN *can;
    std::vector<Button *> face_buttons;
    std::vector<Button *> shift_buttons;
    std::vector<Potentiometer *> potentiometers;
    std::vector<IInput *> inputs;
    RotaryButtonManager *rotary_button_manager;

    // State variables
    uint16_t pot_v;
    uint8_t pot_v_h;
    uint8_t pot_v_l;
    bool special_1;
    bool special_2;

    uint8_t button_status;
    uint8_t modes_status;
    uint8_t screen_btns;
    uint8_t front_btn_status;
    uint8_t rear_btn_status;
    uint8_t special_btn_status;

    /**
     * @brief Set up all input devices.
     */
    void setupInputs();

    /**
     * @brief FreeRTOS task for processing inputs.
     * 
     * @param pvParameters Pointer to task parameters.
     */
    static void inputTask(void *pvParameters);

    /**
     * @brief FreeRTOS task for transmitting CAN messages.
     * 
     * @param pvParameters Pointer to task parameters.
     */
    static void canTransmitTask(void *pvParameters);

    /**
     * @brief Update custom state variables.
     */
    void updateCustomStates();

    /**
     * @brief Prepare a CAN message for transmission.
     * 
     * @param message Reference to the CAN message to be prepared.
     */
    void prepareCANMessage(twai_message_t &message);

    /**
     * @brief Print the current state to the serial port.
     */
    void printSerialMessage();
};

#endif // STEERING_WHEEL_H
