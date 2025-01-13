#include "CarEmulator.h"

CarEmulator::CarEmulator() 
    : t_config(TWAI_TIMING_CONFIG_1MBITS())
    , f_config(TWAI_FILTER_CONFIG_ACCEPT_ALL())
    , g_config(TWAI_GENERAL_CONFIG_DEFAULT(CAN_TX_PIN, CAN_RX_PIN, TWAI_MODE_NORMAL))
{
}

void CarEmulator::setup() {
    initializeCAN();
}

void CarEmulator::start() {
    ESP_ERROR_CHECK(twai_start());
    ESP_LOGI(LOG_TAG, "CAN Driver started");
    createTasks();
}

void CarEmulator::initializeCAN() {
    ESP_ERROR_CHECK(twai_driver_install(&g_config, &t_config, &f_config));
    ESP_LOGI(LOG_TAG, "CAN Driver installed");
}

void CarEmulator::createTasks() {
    xTaskCreatePinnedToCore(receiveTask, "CAN_rx", TASK_STACK_SIZE, this, TASK_PRIORITY_RECEIVE, NULL, tskNO_AFFINITY);
    ESP_LOGI(LOG_TAG, "CAN Receive Task created");
}

void CarEmulator::receiveTask(void* pvParameters) {
    CarEmulator* emulator = static_cast<CarEmulator*>(pvParameters);
    twai_message_t message;

    while (true) {

        if (twai_receive(&message, portMAX_DELAY) == ESP_OK) {
            emulator->printCANMessage(message);
        } else {
            ESP_LOGE(LOG_TAG, "Failed to receive message");
        }

        vTaskDelay(pdMS_TO_TICKS(1));
    }
}

void CarEmulator::printCANMessage(const twai_message_t& message) {
    Serial.print("Message received - ID: 0x");
    Serial.print(message.identifier, HEX);
    Serial.print(", Data: ");
    for (int i = 0; i < message.data_length_code; i++) {
        Serial.print(message.data[i], HEX);
        Serial.print(" ");
    }
    Serial.println();
}
