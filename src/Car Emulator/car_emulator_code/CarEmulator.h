#ifndef CAR_EMULATOR_H
#define CAR_EMULATOR_H

#include "constants.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_err.h"
#include "esp_log.h"
#include "driver/twai.h"

/**
 * @brief A class that emulates a car's CAN communication system.
 *
 * This class provides functionality to set up and run a CAN bus emulator,
 * which can be used to test and debug CAN-based automotive systems.
 */
class CarEmulator {
public:
    /**
     * @brief Construct a new Car Emulator object.
     */
    CarEmulator();

    /**
     * @brief Set up the Car Emulator.
     *
     * This method initializes the CAN driver and creates necessary tasks.
     */
    void setup();

    /**
     * @brief Start the Car Emulator.
     *
     * This method starts the CAN driver and begins the message receiving process.
     */
    void start();

private:
    twai_timing_config_t t_config;
    twai_filter_config_t f_config;
    twai_general_config_t g_config;

    SemaphoreHandle_t rx_sem;
    SemaphoreHandle_t done_sem;

    /**
     * @brief Task for receiving CAN messages.
     *
     * @param pvParameters Pointer to task parameters (usually a pointer to the CarEmulator instance).
     */
    static void receiveTask(void* pvParameters);

    /**
     * @brief Initialize the CAN driver.
     */
    void initializeCAN();

    /**
     * @brief Create FreeRTOS tasks for the emulator.
     */
    void createTasks();

    
    /**
     * @brief Print the received CAN message to the serial monitor.
     * 
     * @param message The received CAN message to be printed.
     */
    void printCANMessage(const twai_message_t& message);
};

#endif // CAR_EMULATOR_H
