#ifndef STEERING_WHEEL_H
#define STEERING_WHEEL_H

#include "ISteeringWheel.h"
#include "ICAN.h"
#include "IInput.h"
#include "RotaryButtonManager.h"
#include "constants.h"
#include <vector>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

class SteeringWheel : public ISteeringWheel
{
public:
    SteeringWheel(ICAN &can);
    void setup() override;
    void start() override;

private:
    // Object variables
    ICAN &can;
    std::vector<IInput *> face_buttons;
    std::vector<IInput *> shift_buttons;
    std::vector<IInput *> potentiometers;
    std::vector<IInput *> inputs;
    RotaryButtonManager rotary_button_manager;

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

    // Private methods
    void setupInputs();
    static void inputTask(void *pvParameters);
    static void canTransmitTask(void *pvParameters);
    void prepareCANMessage(twai_message_t &message);
    void printSerialMessage();
};

#endif // STEERING_WHEEL_H
