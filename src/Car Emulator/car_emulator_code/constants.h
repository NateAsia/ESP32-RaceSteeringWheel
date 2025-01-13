#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <Arduino.h>
#include "driver/twai.h"

// CAN Configuration
constexpr uint32_t CAN_MSG_ID = 0x555;
constexpr uint32_t CAN_BAUD_RATE = 1000000; // 1 Mbps
constexpr gpio_num_t CAN_TX_PIN = GPIO_NUM_6;
constexpr gpio_num_t CAN_RX_PIN = GPIO_NUM_7;

// Task Priorities
constexpr uint8_t TASK_PRIORITY_RECEIVE = 9;
constexpr uint8_t TASK_PRIORITY_TRANSMIT = 8;
constexpr uint8_t TASK_PRIORITY_CONTROL = 10;

// Other Constants
constexpr const char* LOG_TAG = "CAR_EMULATOR";
constexpr uint32_t TASK_STACK_SIZE = 4096;

#endif // CONSTANTS_H
