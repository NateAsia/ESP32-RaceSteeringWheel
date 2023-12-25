/* 
  ******************** CONSTANTS FILE ********************

  - Please use this for all variables that remain constant :)
  - Please label definitions with useful information on the right
  - Add new sections as needed

 */



/* --------------------- ESP-IDF Setup ------------------ */
  // LOG TAGS
    #define BASE_TAG    "TWAI Listen Only"  



/* --------------------- FreeRTOS Setup ------------------ */
  #define TX_TASK_PRIO      9         // TX Task Priority 
  #define RX_TASK_PRIO      8         // RX Task Priority 
  #define INPUT_TASK_PRIO   7         // INPUT Task Priority
  #define OUTPUT_TASK_PRIO  6         // INPUT Task Priority


/* --------------------- TWAI / CAN Transceiver Setup ------------------ */
  #define TX_GPIO_NUM       17        // "TX2" on the ESP32-WROOM-32
  #define RX_GPIO_NUM       16        // "RX2" on the ESP32-WROOM-32


/* --------------------- Steering Wheel Inputs ------------------ */
  // Buttons
    #define BUTTON_1_PIN    22        // Location:? Color:? Purpose: DRS
    #define BUTTON_2_PIN    19   // Location:? Color:? Purpose: Push To Talk
    #define BUTTON_3_PIN    18   // Location:? Color:? Purpose: Launch Control Engage
    #define BUTTON_4_PIN    21   // Location:? Color:? Purpose: Dash Control 
  
  // Rotary Switch
    #define ROTARY_1_PIN        // Location:? Color:? Purpose: Engine Modes

  // Analog Inputs
    #define POT_X_PIN       4     // Location:? Color:?
    #define POT_Y_PIN       15    // Location:? Color:?
    
    #define POT_1_PIN           // Location:? Color:? Purpose: Clutch LEFT
    #define POT_2_PIN           // Location:? Color:? Purpose: Clutch RIGHT


/* --------------------- Steering Wheel Outputs ------------------ */

  // STATUS LEDS
    #define LED_1_PIN           // Location:? Color:? Purpose: BSPD Status
    #define LED_2_PIN           // Location:? Color:? Purpose: Steering Wheel Zero
    #define LED_3_PIN           // Location:? Color:? Purpose: BOTS Status
    // #define LED_4_PIN     // Location:? Color:? Purpose:?
    // #define LED_5_PIN     // Location:? Color:? Purpose:?
    // #define LED_6_PIN     // Location:? Color:? Purpose:?

    #define LED_BUILTIN 2       // Blue / On ESP
    