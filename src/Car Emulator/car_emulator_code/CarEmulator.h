#ifndef CAR_EMULATOR_H
#define CAR_EMULATOR_H

#include "constants.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_err.h"
#include "esp_log.h"
#include "driver/twai.h"

class CarEmulator {
public:
    CarEmulator();
    void setup();
    void start();

private:
    twai_timing_config_t t_config;
    twai_filter_config_t f_config;
    twai_general_config_t g_config;

    SemaphoreHandle_t rx_sem;
    SemaphoreHandle_t done_sem;

    static void receiveTask(void* pvParameters);
    void initializeCAN();
    void createTasks();
    void printCANMessage(const twai_message_t& message);
};

#endif // CAR_EMULATOR_H