#ifndef CONSTANTS
#define CONSTANTS

#include <Arduino.h>

/* --------------------- FreeRTOS Setup ------------------ */
  #define TX_TASK_PRIO      9         // TX Task Priority 
  #define RX_TASK_PRIO      8         // RX Task Priority 
  #define INPUT_TASK_PRIO   7         // INPUT Task Priority
  #define OUTPUT_TASK_PRIO  6         // INPUT Task Priority


/* --------------------- TWAI / CAN Transceiver Setup ------------------ */
  #define CAN_MSG_ID            0x661                     // CAN Message ID
  #define CAN_MSG_FREQUENCY     100                       // Message Frequency (Hz)
  #define CAN_MESSAGE_PERIOD_MS (1000/CAN_MSG_FREQUENCY)  // Message Period (ms)

  #define INTERTED_LOGIC  true

/* --------------------- PINS ------------------ */
  // Buttons
    #define BUTTON_1_PIN    4     // Location: LEFT-TOP   Color: BLUE 
    #define BUTTON_2_PIN    5     // Location: LEFT-MID   Color: RED 
    #define BUTTON_3_PIN    7     // Location: RIGHT-MID  Color: RED 
    #define BTN_REAR_1      8     // Location: LEFT-REAR  (PADDLE)
    #define BTN_REAR_2      9     // Location: RIGHT-REAR (PADDLE)
  
  // Rotary Switch
    #define ROTARY_BTN_1_PIN  6   // Location:LEFT-LOW  Color: YELLOW 
    #define ROTARY_BTN_2_PIN  10  // Location:RIGHT-LOW Color: YELLOW

  // Analog Inputs
    #define POT_1_PIN         D1  // Location:L-R-POT 
    #define POT_2_PIN         D0  // Location:R-R-POT 

  // CAN Inputs
    #define TX_GPIO_NUM       D6  // "TX2" on the ESP32-C3
    #define RX_GPIO_NUM       D7  // "RX2" on the ESP32-C3

    #define LED_BUILTIN       2   // Blue / On ESP *** not on ESPC3

/* --------------------- Steering Wheel Outputs ------------------ */

    #define ROTARY_BTN_LIST_SIZE  2   // Number of rotary buttons 
                                      // (TODO: Covert rotary into object, and automatically detect number of buttens)

/* --------------------- SOFTWARE DEFS ------------------ */

  // TIME
    #define ROTARY_NO_INPUT_TIME 5000   // 5000 ms
    #define ROTARY_WAIT_TIME 2000       // 2000 ms
    #define RTY_BTN_DEBOUNCE 40         //   40 ms
    #define RTY_BTN_DEBOUNCE_LONG 100   //   100 ms

  // EEPROM
    #define EEPROM_SIZE 16              // Number of bytes we'll use in eeprom
    #define RTY_1_EE_LOCATION 0
    #define RTY_2_EE_LOCATION 1

#endif // CONSTANTS

    