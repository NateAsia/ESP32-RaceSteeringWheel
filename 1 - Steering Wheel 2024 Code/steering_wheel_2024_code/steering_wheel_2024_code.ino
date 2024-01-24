/* --------------------- Standard / ESP Specific Imports ------------------ */ // FYI use (command + click) on a import or variable to goto its definition for more info 
  #include <stdio.h>                // Yummy OS Stuff
  #include <stdlib.h>               // Alot of helpful yummy libs
  #include <EEPROM.h>               // Library to read and write from flash memory
  #include "freertos/FreeRTOS.h"    // Task Management
  #include "freertos/task.h"        // Task Management
  #include "freertos/queue.h"       // Task Management 
  #include "freertos/semphr.h"      // Task Management
  #include "esp_err.h"              // ESP32 Errors     --  used for debugging
  #include "esp_log.h"              // ESP32 Logging    --  used for debugging
  #include "driver/twai.h"          // API HERE --> [https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/twai.html]

/* --------------------- Definitions and static variables ------------------ */

  #include "constants.h"    // Project Constants    #TODO #1 in this file :) ***** ***** ***** *****

  static const twai_filter_config_t   f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();   // We can filter our CAN reception to specific addresses only (later)
  static const twai_timing_config_t   t_config = TWAI_TIMING_CONFIG_1MBITS();       // CAN SPEED (must change to 1000Kbits to work with our MoTeC System)         SPEED... I AM SPEED
  static const twai_general_config_t  g_config = TWAI_GENERAL_CONFIG_DEFAULT((gpio_num_t)TX_GPIO_NUM, (gpio_num_t)RX_GPIO_NUM, TWAI_MODE_NO_ACK);

  static SemaphoreHandle_t tx_sem;


/* --------------------- DYNAMIC VARIABLES (I/O Status) ------------------ */

  // CAN
    twai_message_t message;         // CAN Message Struct

  // TODO #2: ADD ALL VARIABLES FOR CAR STEERING WHEEL ***** ***** ***** ***** ***** ***** 
    bool      button_1  = false;     // Matches to the status of BUTTON_1_PIN :)
    bool      button_2  = false;     // Matches to the status of BUTTON_2_PIN :)
    bool      button_3  = false;     // Matches to the status of BUTTON_3_PIN :)
    bool      button_4  = false;     // Matches to the status of BUTTON_4_PIN :)

    uint8_t   button_status = 0b00000000;   // = 0

    ROTARY_BTN left_rty_btn   = { 
      .state            = false,
      .last_state       = false, 
      .time_hold_start  = 0, 
      .mode             = 0, 
      .num_of_modes     = 4};
    ROTARY_BTN right_rty_btn  = {
      .state            = false,
      .last_state       = false, 
      .time_hold_start  = 0, 
      .mode             = 0, 
      .num_of_modes     = 4};
    bool rty_update_mode      = false;
    bool dual_hold            = false;
    bool holding_too_long     = false;
    long hold_start_time      = 0;
    ROTARY_BTN *r_list[ROTARY_BTN_LIST_SIZE] = {&left_rty_btn, &right_rty_btn};
  
    uint16_t  pot_1     = 0x0000;      // Mapped from 0 - 255 of the status of POT_1_PIN
    uint8_t   pot_1_h   = 0x00;
    uint8_t   pot_1_l   = 0x00;

    uint16_t  pot_2     = 0x0000;      // Mapped from 0 - 255 of the status of POT_2_PIN
    uint8_t   pot_2_h   = 0x00;
    uint8_t   pot_2_l   = 0x00;


/* --------------------------- Tasks and Functions -------------------------- */

void setup_io(){
  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(BUTTON_1_PIN, INPUT_PULLUP);  
  pinMode(BUTTON_2_PIN, INPUT_PULLUP);
  pinMode(BUTTON_3_PIN, INPUT_PULLUP);
  pinMode(BUTTON_4_PIN, INPUT_PULLUP);

  pinMode(ROTARY_BTN_1_PIN, INPUT_PULLUP);
  pinMode(ROTARY_BTN_2_PIN, INPUT_PULLUP);

  pinMode(POT_1_PIN, INPUT);
  pinMode(POT_2_PIN, INPUT);
}

void rotary_input_check(){

  left_rty_btn.state  = !digitalRead(ROTARY_BTN_1_PIN);
  right_rty_btn.state = !digitalRead(ROTARY_BTN_2_PIN);

  // Entering and Exiting Rotary Update Mode
    if(left_rty_btn.state & left_rty_btn.state){
      if(!dual_hold){
        dual_hold = true;
        hold_start_time = millis();
      }
      else{
        if(!holding_too_long && millis() - hold_start_time >= ROTARY_WAIT_TIME){
          rty_update_mode = !rty_update_mode;
          holding_too_long = true;
          if(!rty_update_mode){     // When we exit rotary update mode, update the EEPROM
              EEPROM.writeByte(RTY_1_EE_LOCATION, left_rty_btn.mode);
              EEPROM.writeByte(RTY_2_EE_LOCATION, right_rty_btn.mode);
              EEPROM.commit();
          }
        }
        return;  // if holding both, do not continue reading for mode swichtes
      }
    }
    else{
      dual_hold = false;
      holding_too_long = false;
    }

  // When in Rotary Update Mode, update external modes with a button press (rising edge)
    if (rty_update_mode){
      for(int i = 0; i < ROTARY_BTN_LIST_SIZE; i++){

        // Start with debouncing the button
          if (r_list[i]->last_state != r_list[i]->state){
            r_list[i]->time_hold_start = millis();
          }
        
        // Once Debounced, add to the mode counter if the button state is high (pressed)
        if (millis() - r_list[i]->time_hold_start > RTY_BTN_DEBOUNCE){
          r_list[i]->mode += r_list[i]->state;

          if (r_list[i]->mode == r_list[i]->num_of_modes){
            r_list[i]->mode = 0;
          }
        }
      }
    }
}


// Thread to update the variables corresponding to steering wheel inputs (every 1ms)
static void update_input_statuses(void *arg){

  
  ESP_LOGI(BASE_TAG, "Update Inputs Thread Started");
  
  while(1){
      
      // TODO: check on all of the inputs and update the dynamic variables ***** ***** ***** 

      rotary_input_check();

      button_1  = !digitalRead(BUTTON_1_PIN);   // (!) since we are using a pullup network on buttons
      button_2  = !digitalRead(BUTTON_2_PIN);
      button_3  = !digitalRead(BUTTON_3_PIN);
      button_4  = !digitalRead(BUTTON_4_PIN);
    
      button_status = (button_4 << 3) | (button_3 << 2) | (button_2 << 1) | button_1;

      pot_1     = analogReadMilliVolts(POT_1_PIN);        // Returns    12 Bits Total
      pot_1_h   = (pot_1 & 0xFF00) >> 8;        // High Bits  4 Bits get used (out of 8)
      pot_1_l   = (pot_1 & 0x00FF);             // Low Bits   8 Bits

      pot_2     = analogReadMilliVolts(POT_2_PIN);
      pot_2_h   = (pot_2 & 0xFF00) >> 8;        // High Bits        /// 00001111
      pot_2_l   = (pot_2 & 0x00FF);             // Low Bits 


    vTaskDelay(pdMS_TO_TICKS(1));    // This will repeat every 1 ms
  }

  vTaskDelete(NULL);
}

static void twai_transmit_task(void *arg)
{
    xSemaphoreTake(tx_sem, portMAX_DELAY);
    ESP_LOGI(BASE_TAG, "Transmit Thread Started");

    while(1){
      digitalWrite(LED_BUILTIN, HIGH);

      // TODO #3: MAKE TX TASK
      //    this should just read variables (not the pins directly), assign them to their specified address / data slot, then send
      // 
      // NOTE: a single address contains 8 Bytes for data (thats alot!) so we can likly fit everything into 1 or 2 addresses
      //    look at the CAN analysis to see which addresses are not in use or can be taken
      // 
      // Just get a feel for this, and we can optimize it later :)
      
        twai_message_t message1 = {
                                    .identifier = 0x7F9, 
                                    .data_length_code = 5,
                                    .data = {
                                      button_status, 
                                      pot_1_h, 
                                      pot_1_l, 
                                      pot_2_h, 
                                      pot_2_l
                                      } 
                                  };

        ESP_ERROR_CHECK(twai_transmit(&message1 , portMAX_DELAY));  // Force the ESP to restart if there is a transmitted error

      digitalWrite(LED_BUILTIN, LOW);
      vTaskDelay(pdMS_TO_TICKS(10));    // Send updates @ 100 Hz
    }

    xSemaphoreGive(tx_sem);
    vTaskDelete(NULL);
}

void app_main(void){

    // Create Semaphore(s) 
    tx_sem = xSemaphoreCreateBinary();  

    // CREATE THREADS (TASKS)
    xTaskCreatePinnedToCore(twai_transmit_task,   "TWAI_tx",        4096, NULL, TX_TASK_PRIO,     NULL, tskNO_AFFINITY);
    xTaskCreatePinnedToCore(update_input_statuses,"UPDATE_INPUTS",  4096, NULL, INPUT_TASK_PRIO,  NULL, tskNO_AFFINITY);

    // Install and start TWAI driver            -   This will force the ESP32 to restart if there is a CAN error - good
    ESP_ERROR_CHECK(twai_driver_install(&g_config, &t_config, &f_config));
    ESP_LOGI(BASE_TAG, "CAN Driver installed");
    ESP_ERROR_CHECK(twai_start());
    ESP_LOGI(BASE_TAG, "CAN Driver started");

    xSemaphoreGive(tx_sem);                     // Allow Start of TX task
    vTaskDelay(pdMS_TO_TICKS(100));             
    
    xSemaphoreTake(tx_sem, portMAX_DELAY);      // Wait for TX task to complete (never ;) )
    printf("\nEXITING\n");                      // (we will NEVER reach this line)

    //Stop and uninstall TWAI driver            // Just incase tho
    ESP_ERROR_CHECK(twai_stop());
    ESP_ERROR_CHECK(twai_driver_uninstall());
    ESP_LOGI(BASE_TAG, "CAN Driver Uninstalled");

    //Cleanup
    vSemaphoreDelete(tx_sem);
}

/* --------------------------- Arduino IDE Compliance --------------------------- */
void setup(){
  
  // Don't use use Serial.begin --> Assume serial is running on 115200 and use the printf function its MUCH faster
  ESP_LOGI(BASE_TAG, "ESP-STARTING");
  
  setup_io();

  EEPROM.begin(EEPROM_SIZE); // Setup Flash Memory

  left_rty_btn.mode   = EEPROM.readByte(RTY_1_EE_LOCATION);  // Read Last mode of rty_position from memory on boot
  right_rty_btn.mode  = EEPROM.readByte(RTY_2_EE_LOCATION);  // Read Last mode of rty_position from memory on boot

  printf("\n\t\tInitializing CAN\n\n");
  
  // Call the ESP-IDF specific startup procedure
  app_main();

}

void loop(){
  // do nothing in this loop, the FreeRTOS is managing task scheduling above
}
