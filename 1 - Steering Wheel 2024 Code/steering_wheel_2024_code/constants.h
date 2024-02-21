/* 
  ******************** CONSTANTS FILE ********************

  - Please use this for all variables that remain constant :)
  - Please label definitions with useful information on the right
  - Add new sections as needed

 */



/* --------------------- ESP-IDF Setup ------------------ */
  // LOG TAGS
    #define BASE_TAG    "DUKE"  


/* --------------------- FreeRTOS Setup ------------------ */
  #define TX_TASK_PRIO      9         // TX Task Priority 
  #define RX_TASK_PRIO      8         // RX Task Priority 
  #define INPUT_TASK_PRIO   7         // INPUT Task Priority
  #define OUTPUT_TASK_PRIO  6         // INPUT Task Priority


/* --------------------- TWAI / CAN Transceiver Setup ------------------ */
  #define TX_GPIO_NUM       D6        // "TX2" on the ESP32-C3
  #define RX_GPIO_NUM       D7        // "RX2" on the ESP32-C3


/* --------------------- Steering Wheel Inputs ------------------ */
  // Buttons
    #define BUTTON_1_PIN    4   // Location: LEFT-TOP   Color: BLUE 
    #define BUTTON_2_PIN    5   // Location: LEFT-MID   Color: RED 

    #define BUTTON_3_PIN    7   // Location: RIGHT-MID  Color: RED 

    #define BTN_REAR_1      8   // Location: LEFT-REAR  (PADDLE)
    #define BTN_REAR_2      9   // Location: RIGHT-REAR (PADDLE)
  
  // Rotary Switch
    #define ROTARY_BTN_LIST_SIZE  2
    #define ROTARY_BTN_1_PIN      6   // Location:LEFT-LOW  Color: YELLOW 
    #define ROTARY_BTN_2_PIN      10  // Location:RIGHT-LOW Color: YELLOW

  // Analog Inputs
    #define POT_1_PIN D1   // Location:L-R-POT 
    #define POT_2_PIN D0   // Location:R-R-POT 


/* --------------------- Steering Wheel Outputs ------------------ */

    #define LED_BUILTIN 2       // Blue / On ESP *** not on ESPC3

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

/* --------------------- Structs ------------------ */

  /* Rotary Button Struct */
    typedef struct ROTARY_BTN_T { 
      bool    state;
      bool    last_state;
      bool    disable;
      long    time_hold_start;
      uint8_t mode;
      uint8_t num_of_modes;
    } ROTARY_BTN; 


    