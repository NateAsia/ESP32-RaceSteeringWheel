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
    #define BUTTON_1_PIN    4    // Location:? Color:? Purpose: DRS <-- may be automated
    #define BUTTON_2_PIN    5   // Location:? Color:? Purpose: Push To Talk <-- direct control
    #define BUTTON_3_PIN    7    // Location:? Color:? Purpose: Launch Control Engage

    #define BTN_REAR_1      8    // Location:? Color:? Purpose: Dash Control 
    #define BTN_REAR_2      9    // Location:? Color:? Purpose: Dash Control 
  
  // Rotary Switch
    #define ROTARY_BTN_LIST_SIZE  2
    #define ROTARY_BTN_1_PIN      6 // Location: ? Color: ? Purpose: 2 sec hold 
    #define ROTARY_BTN_2_PIN      10 // Location: ? Color: ? Purpose:  

  // Analog Inputs
    #define POT_1_PIN D0   // Location:? Color:? Purpose: Clutch LEFT
    #define POT_2_PIN D1   // Location:? Color:? Purpose: Clutch RIGHT


/* --------------------- Steering Wheel Outputs ------------------ */

    #define LED_BUILTIN 2       // Blue / On ESP

/* --------------------- SOFTWARE DEFS ------------------ */

  // TIME
    #define ROTARY_WAIT_TIME 2000 // 2000 ms
    #define RTY_BTN_DEBOUNCE 40   //   40 ms
    #define RTY_BTN_DEBOUNCE_LONG 100   //   40 ms

  // EEPROM
    #define EEPROM_SIZE 16 // Number of bytes we'll use in eeprom
    #define RTY_1_EE_LOCATION 0
    #define RTY_2_EE_LOCATION 1

/* --------------------- Structs ------------------ */

  // // Rotary BTNs
    typedef struct ROTARY_BTN_T { 
      bool    state;
      bool    last_state;
      bool    disable;
      long    time_hold_start;
      uint8_t mode;
      uint8_t num_of_modes;
    } ROTARY_BTN; 


    