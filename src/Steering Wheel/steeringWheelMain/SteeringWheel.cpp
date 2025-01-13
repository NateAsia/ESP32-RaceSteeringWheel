#include "Print.h"
#include "SteeringWheel.h"
#include "stdio.h"
#include "esp_log.h"
#include <EEPROM.h>
#include "constants.h"
#include "Button.h"
#include "Potentiometer.h"

SteeringWheel::SteeringWheel(ICAN *can) : can(can) {}

void SteeringWheel::setup()
{
    setupInputs();
    ESP_LOGI("SteeringWheel", "Steering Wheel Setup Complete");
}

void SteeringWheel::start()
{
    // Create tasks
    xTaskCreatePinnedToCore(inputTask, "InputTask", 4096, this, INPUT_TASK_PRIO, NULL, tskNO_AFFINITY);
    xTaskCreatePinnedToCore(canTransmitTask, "CANTransmitTask", 4096, this, TX_TASK_PRIO, NULL, tskNO_AFFINITY);
    ESP_LOGI("SteeringWheel", "Steering Wheel Starting");
}

void SteeringWheel::setupInputs()
{
    face_buttons.push_back(new Button(BUTTON_1_PIN, INTERTED_LOGIC));
    face_buttons.push_back(new Button(BUTTON_2_PIN, INTERTED_LOGIC));
    face_buttons.push_back(new Button(BUTTON_3_PIN, INTERTED_LOGIC));
    face_buttons.push_back(new Button(ROTARY_BTN_1_PIN, INTERTED_LOGIC));
    face_buttons.push_back(new Button(ROTARY_BTN_2_PIN, INTERTED_LOGIC));

    shift_buttons.push_back(new Button(BTN_REAR_1, INTERTED_LOGIC));
    shift_buttons.push_back(new Button(BTN_REAR_2, INTERTED_LOGIC));

    potentiometers.push_back(new Potentiometer(POT_1_PIN));
    potentiometers.push_back(new Potentiometer(POT_2_PIN));

    rotary_button_manager = new RotaryButtonManager(&(face_buttons[3]->state), &(face_buttons[4]->state));

    inputs.insert(inputs.end(), face_buttons.begin(), face_buttons.end());
    inputs.insert(inputs.end(), shift_buttons.begin(), shift_buttons.end());
    inputs.insert(inputs.end(), potentiometers.begin(), potentiometers.end());
    inputs.push_back(rotary_button_manager);

    for (auto& input : inputs)
    {
        input->setup();
    }
    ESP_LOGI("SteeringWheel", "Inputs Setup Complete");
}

void SteeringWheel::inputTask(void *pvParameters)
{
    SteeringWheel *wheel = static_cast<SteeringWheel *>(pvParameters);
    while (1)
    {
        for (auto& input : wheel->inputs)
        {
            input->update_state(); // Update all input states
        }

        wheel->pot_v = max(wheel->potentiometers[0]->state, wheel->potentiometers[1]->state);
        wheel->pot_v_h = (wheel->pot_v & 0xFF00) >> 8; // High Bits
        wheel->pot_v_l = (wheel->pot_v & 0x00FF);      // Low Bits

        wheel->front_btn_status =
            (wheel->face_buttons[0]->state << 7) | (wheel->face_buttons[1]->state << 6) | (wheel->face_buttons[2]->state << 5);

        wheel->rear_btn_status =
            (wheel->shift_buttons[0]->state << 4) | (wheel->shift_buttons[1]->state << 3);

        wheel->special_btn_status =
            (wheel->special_1 << 2) | (wheel->special_2 << 1);

        wheel->button_status =
            wheel->front_btn_status | wheel->rear_btn_status | wheel->special_btn_status | wheel->rotary_button_manager->in_rty_update_mode();

        wheel->modes_status = wheel->rotary_button_manager->get_modes_status();
        wheel->screen_btns = wheel->rotary_button_manager->get_screen_btns();

        vTaskDelay(pdMS_TO_TICKS(1));
    }
}

void SteeringWheel::updateCustomStates()
{
    special_1 =
        (potentiometers[0]->state > 2600) & (potentiometers[1]->state > 2600) & rotary_button_manager->in_left_special_btn_state();

    special_2 =
        (potentiometers[0]->state > 2600) & (potentiometers[1]->state > 2600) & rotary_button_manager->in_right_special_btn_state();
}

void SteeringWheel::canTransmitTask(void *pvParameters)
{
    SteeringWheel *wheel = static_cast<SteeringWheel *>(pvParameters);
    twai_message_t message;

    while (1)
    {
        wheel->prepareCANMessage(message);
        wheel->can->transmit(message);
        wheel->printSerialMessage();
        vTaskDelay(pdMS_TO_TICKS(CAN_MESSAGE_PERIOD_MS));
    }
}

void SteeringWheel::prepareCANMessage(twai_message_t &message)
{
    // Prepare CAN message based on message states
    message.identifier = CAN_MSG_ID;
    message.data_length_code = 8;
    
    // Copy values into message.data array
    message.data[0] = this->button_status;
    message.data[1] = this->modes_status;
    message.data[2] = this->screen_btns;
    message.data[3] = this->pot_v_h;
    message.data[4] = this->pot_v_l;
    message.data[5] = 0;  // Placeholder for unused data
    message.data[6] = 0;  // Placeholder for unused data
    message.data[7] = 0;  // Placeholder for unused data
}

void SteeringWheel::printSerialMessage(){
  Serial.print("<!-- STATUS -->");
  Serial.print(" BTNS: ");
  Serial.print(face_buttons[0]->state);
  Serial.print("-");
  Serial.print(face_buttons[1]->state);
  Serial.print("-");
  Serial.print(face_buttons[3]->state);
  Serial.print("-");

  Serial.print(face_buttons[2]->state);
  Serial.print("-");
  Serial.print(face_buttons[4]->state);
  Serial.print("-");

  Serial.print(shift_buttons[0]->state);
  Serial.print("-");
  Serial.print(shift_buttons[1]->state);

  Serial.print(" POTS: ");
  Serial.print(potentiometers[0]->state);
  Serial.print("-");
  Serial.print(potentiometers[1]->state);
  Serial.print("-");
  Serial.print(pot_v);

  Serial.print(" MODES: ");
  Serial.print(rotary_button_manager->in_rty_update_mode());
  Serial.print("-");
  Serial.print(rotary_button_manager->in_dual_hold());
  Serial.print("-");
  Serial.print((modes_status & 0xF0) >>4);
  Serial.print("-");
  Serial.print(modes_status & 0x0F);
  Serial.print("-");
  Serial.print(special_1);
  Serial.print("-");
  Serial.print(special_2);
  Serial.print("\n");
}
