#include "SteeringWheel.h"
#include "stdio.h"
#include "esp_log.h"
#include <EEPROM.h>
#include "constants.h"

SteeringWheel::SteeringWheel(ICAN &can) : can(can)
{
    inputs.push_back(&face_buttons);
    inputs.push_back(&shift_buttons);
    inputs.push_back(&potentiometers);
    inputs.push_back(&rotary_button_manager);
}

void SteeringWheel::setup()
{
    setupInputs();
    ESP_LOGI(BASE_TAG, "Steering Wheel Setup Complete");
}

void SteeringWheel::start()
{
    // Create tasks
    xTaskCreatePinnedToCore(inputTask, "InputTask", 4096, this, INPUT_TASK_PRIO, NULL, tskNO_AFFINITY);
    xTaskCreatePinnedToCore(canTransmitTask, "CANTransmitTask", 4096, this, TX_TASK_PRIO, NULL, tskNO_AFFINITY);
    ESP_LOGI(BASE_TAG, "Steering Wheel Starting");
}

void SteeringWheel::setupInputs()
{
    face_buttons.push_back(new Button(BUTTON_1_PIN));
    face_buttons.push_back(new Button(BUTTON_2_PIN));
    face_buttons.push_back(new Button(BUTTON_3_PIN));
    face_buttons.push_back(new Button(ROTARY_BTN_1_PIN));
    face_buttons.push_back(new Button(ROTARY_BTN_2_PIN));

    shift_buttons.push_back(new Button(BTN_REAR_1));
    shift_buttons.push_back(new Button(BTN_REAR_2));

    potentiometers.push_back(new Potentiometer(POT_1_PIN));
    potentiometers.push_back(new Potentiometer(POT_2_PIN));

    rotary_button_manager = RotaryButtonManager(&face_buttons[3]->state, &face_buttons[4]->state);

    for (auto input : inputs)
    {
        input->setup();
    }
    ESP_LOGI(BASE_TAG, "Inputs Setup Complete");
}

void SteeringWheel::inputTask(void *pvParameters)
{
    SteeringWheel *wheel = static_cast<SteeringWheel *>(pvParameters);
    while (1)
    {
        for (auto input : wheel->inputs)
        {
            input->update_state(); // Update all input states
        }

        pot_v = max(potentiometers[0]->state, potentiometers[1]->state);
        pot_v_h = (pot_v & 0xFF00) >> 8; // High Bits
        pot_v_l = (pot_v & 0x00FF);      // Low Bits

        front_btn_status =
            (face_buttons[0]->state << 7) | (face_buttons[1]->state << 6) | (face_buttons[2]->state << 5);

        rear_btn_status =
            (btn_rear_1 << 4) | (btn_rear_2 << 3);

        special_btn_status =
            (special_1 << 2) | (special_2 << 1);

        button_status =
            front_btn_status | rear_btn_status | special_btn_status | rotary_button_manager.get_in_rty_update_mode();

        modes_status = rotary_button_manager.get_modes_status();
        screen_btns = rotary_button_manager.get_screen_btns();

        vTaskDelay(pdMS_TO_TICKS(1));
    }
}

void SteeringWheel::updateCustomStates()
{
    special_1 =
        (potentiometers[0]->state > 2600) & (potentiometers[1]->state > 2600) & rotary_button_manager.in_left_special_btn_state();

    special_2 =
        (potentiometers[0]->state > 2600) & (potentiometers[1]->state > 2600) & rotary_button_manager.in_right_special_btn_state();
}

void SteeringWheel::canTransmitTask(void *pvParameters)
{
    SteeringWheel *wheel = static_cast<SteeringWheel *>(pvParameters);
    twai_message_t message;

    while (1)
    {
        wheel->prepareCANMessage(message);
        wheel->can.transmit(message);
        wheel->printSerialMessage();
        vTaskDelay(pdMS_TO_TICKS(CAN_MESSAGE_PERIOD_MS));
    }
}

void SteeringWheel::prepareCANMessage(twai_message_t &message)
{
    // Prepare CAN message based on message states
    message.identifier = CAN_MSG_ID;
    message.data_length_code = 8;
    message.data = {
        button_status,
        modes_status,
        screen_btns,
        pot_v_h,
        pot_v_l,
        0,
        0,
        0
    }
}

void SteeringWheel::printSerialMessage(){
  Serial.print("BTNS:");
  Serial.print(face_buttons[0]->state);
  Serial.print("@");
  Serial.print(face_buttons[1]->state);
  Serial.print("@");
  Serial.print(face_buttons[3]->state);
  Serial.print("@");

  Serial.print(face_buttons[2]->state);
  Serial.print("@");
  Serial.print(face_buttons[4]->state);
  Serial.print("@");

  Serial.print(shift_buttons[0]->state);
  Serial.print("@");
  Serial.print(shift_buttons[1]->state);

  Serial.print(":POTS:");
  Serial.print(potentiometers[0]->state);
  Serial.print("@");
  Serial.print(potentiometers[1]->state);
  Serial.print("@");
  Serial.print(pot_v);

  Serial.print(":MODES:");
  Serial.print(rotary_button_manager.in_rty_update_mode());
  Serial.print("@");
  Serial.print(rotary_button_manager.in_dual_hold());
  Serial.print("@");
  Serial.print((modes_status & 0xF0) >>4);
  Serial.print("@");
  Serial.print(modes_status & 0x0F);
  Serial.print("@");
  Serial.print(special_1);
  Serial.print("@");
  Serial.print(special_2);
  Serial.print("\n");
}
