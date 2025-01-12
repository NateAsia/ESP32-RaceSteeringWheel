#include "CANHandler.h"
#include "esp_log.h"
#include "constants.h"

const twai_general_config_t CANHandler::g_config = TWAI_GENERAL_CONFIG_DEFAULT((gpio_num_t)TX_GPIO_NUM, (gpio_num_t)RX_GPIO_NUM, TWAI_MODE_NO_ACK);
const twai_timing_config_t CANHandler::t_config = TWAI_TIMING_CONFIG_1MBITS();
const twai_filter_config_t CANHandler::f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();

CANHandler::CANHandler() {}

void CANHandler::setup()
{
    ESP_ERROR_CHECK(twai_driver_install(&g_config, &t_config, &f_config));
    ESP_ERROR_CHECK(twai_start());
    ESP_LOGI(BASE_TAG, "CAN Driver installed and started");
}

void CANHandler::transmit(const twai_message_t &message)
{
    ESP_ERROR_CHECK(twai_transmit(&message, portMAX_DELAY));
}
