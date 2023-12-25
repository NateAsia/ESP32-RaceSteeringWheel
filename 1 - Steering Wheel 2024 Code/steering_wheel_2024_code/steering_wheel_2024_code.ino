/* --------------------- Standard / ESP Specific Imports ------------------ */ // FYI use (command + click) on a import or variable to goto its definition for more info 
  #include <stdio.h>                // Yummy OS Stuff
  #include <stdlib.h>               // Alot of helpful yummy libs
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
  static const twai_timing_config_t   t_config = TWAI_TIMING_CONFIG_1MBITS();     // CAN SPEED (must change to 1000Kbits to work with our MoTeC System)         SPEED... I AM SPEED
  static const twai_general_config_t  g_config = TWAI_GENERAL_CONFIG_DEFAULT((gpio_num_t)TX_GPIO_NUM, (gpio_num_t)RX_GPIO_NUM, TWAI_MODE_NO_ACK);

  static SemaphoreHandle_t rx_sem;
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
  
    uint16_t  pot_x     = 0x0000;      // Mapped from 0 - 255 of the status of POT_1_PIN
    uint8_t   pot_x_h   = 0x00;
    uint8_t   pot_x_l   = 0x00;

    uint16_t  pot_y     = 0x0000;
    uint8_t   pot_y_h   = 0x00;
    uint8_t   pot_y_l   = 0x00;


/* --------------------------- Tasks and Functions -------------------------- */

void setup_io(){
  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(BUTTON_1_PIN, INPUT_PULLUP);  
  pinMode(BUTTON_2_PIN, INPUT_PULLUP);
  pinMode(BUTTON_3_PIN, INPUT_PULLUP);
  pinMode(BUTTON_4_PIN, INPUT_PULLUP);

  pinMode(POT_X_PIN, INPUT);
  pinMode(POT_Y_PIN, INPUT);


  // TODO: Setup IO's
    // ex: pinMode(LED_1_PIN, OUTPUT);

}

// Thread to update the variables corresponding to steering wheel inputs
static void update_input_statuses(void *arg){

  
  ESP_LOGI(BASE_TAG, "Update Inputs Thread Started");
  
  while(1){
      
      // TODO: check on all of the inputs and update the dynamic variables ***** ***** ***** 

      button_1  = !digitalRead(BUTTON_1_PIN);   // (!) since we are using a pullup network on buttons
      button_2  = !digitalRead(BUTTON_2_PIN);
      button_3  = !digitalRead(BUTTON_3_PIN);
      button_4  = !digitalRead(BUTTON_4_PIN);
    
      button_status = (button_4 << 3) | (button_3 << 2) | (button_2 << 1) | button_1;

      pot_x     = analogReadMilliVolts(POT_X_PIN);        // Returns    12 Bits Total
      pot_x_h   = (pot_x & 0xFF00) >> 8;        // High Bits  4 Bits get used (out of 8)
      pot_x_l   = (pot_x & 0x00FF);             // Low Bits   8 Bits

      pot_y     = analogReadMilliVolts(POT_Y_PIN);
      pot_y_h   = (pot_y & 0xFF00) >> 8;        // High Bits        /// 00001111
      pot_y_l   = (pot_y & 0x00FF);             // Low Bits 


    vTaskDelay(pdMS_TO_TICKS(1));    // This will repeat every 1 ms
  }

  vTaskDelete(NULL);
}

static void update_output(void *arg){

  ESP_LOGI(BASE_TAG, "Update Outputs Thread Started");
  
  while(1){
      
      // TODO: Update any outputs 

        //digitalWrite(LED_1_PIN, led_1_status);

    vTaskDelay(pdMS_TO_TICKS(10));    // This will repeat every 10 ms
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
                                      pot_x_h, 
                                      pot_x_l, 
                                      pot_y_h, 
                                      pot_y_l
                                      } 
                                  };

        ESP_ERROR_CHECK(twai_transmit(&message1 , portMAX_DELAY));  // Force the ESP to restart if there is a transmitted error

      digitalWrite(LED_BUILTIN, LOW);
      vTaskDelay(pdMS_TO_TICKS(10));    // Send updates @ 100 Hz
    }

    xSemaphoreGive(tx_sem);
    vTaskDelete(NULL);
}

static void twai_receive_task(void *arg){

    xSemaphoreTake(rx_sem, portMAX_DELAY);
    ESP_LOGI(BASE_TAG, "Receive Task Started");

    while(1){

        if (twai_receive(&message, portMAX_DELAY) == ESP_OK); // NOTE: possibly decrease this delay later
        else {
            printf("\nFailed to receive message\n");
            vTaskDelay(1);
            continue;
        }

        // Print received message
          if (message.extd) {
              printf("\n\nExtended Format\tID: 0x%.08x\t", message.identifier);
          } else {
              printf("\n\nStandard Format\tID: 0x%.03x\t", message.identifier);
          }
          if (!(message.rtr)) {
              printf("Data (%d) (hex): ", message.data_length_code);
              for (int i = 0; i < message.data_length_code; i++) {
                  printf("0x%.02x ", message.data[i]);
              }
          }

        //Process received message

          // Later we will use this section to update status of LEDs on the steering wheel ;)
        
        vTaskDelay(1);  // This will allow the CPU to switch to another task :)
    }

    xSemaphoreGive(rx_sem);   // We should never reach this line :)
    vTaskDelete(NULL);

}

void app_main(void){

    // Create Semaphore(s)
    rx_sem = xSemaphoreCreateBinary();  
    tx_sem = xSemaphoreCreateBinary();  

    // CREATE THREADS (TASKS)
    // xTaskCreatePinnedToCore(twai_receive_task,    "TWAI_rx",        4096, NULL, RX_TASK_PRIO,     NULL, tskNO_AFFINITY);
    xTaskCreatePinnedToCore(twai_transmit_task,   "TWAI_tx",        4096, NULL, TX_TASK_PRIO,     NULL, tskNO_AFFINITY);
    xTaskCreatePinnedToCore(update_input_statuses,"UPDATE_INPUTS",  4096, NULL, INPUT_TASK_PRIO,  NULL, tskNO_AFFINITY);
    xTaskCreatePinnedToCore(update_output,        "UPDATE_OUTPUTS", 4096, NULL, OUTPUT_TASK_PRIO, NULL, tskNO_AFFINITY);

    // Install and start TWAI driver            -   This will force the ESP32 to restart if there is a CAN error - good
    ESP_ERROR_CHECK(twai_driver_install(&g_config, &t_config, &f_config));
    ESP_LOGI(BASE_TAG, "CAN Driver installed");
    ESP_ERROR_CHECK(twai_start());
    ESP_LOGI(BASE_TAG, "CAN Driver started");

    xSemaphoreGive(rx_sem);                     // Allow Start of RX task   
    xSemaphoreGive(tx_sem);                     // Allow Start of TX task
    vTaskDelay(pdMS_TO_TICKS(100));             
    
    xSemaphoreTake(rx_sem, portMAX_DELAY);      // Wait for RX Task to complete (never ;) )
    xSemaphoreTake(tx_sem, portMAX_DELAY);      // Wait for TX task to complete (never ;) )
    printf("\nEXITING\n");                      // (we will NEVER reach this line)

    //Stop and uninstall TWAI driver            // Just incase tho
    ESP_ERROR_CHECK(twai_stop());
    ESP_ERROR_CHECK(twai_driver_uninstall());
    ESP_LOGI(BASE_TAG, "CAN Driver Uninstalled");

    //Cleanup
    vSemaphoreDelete(rx_sem);
    vSemaphoreDelete(tx_sem);
}

/* --------------------------- Arduino IDE Compliance --------------------------- */
void setup(){
  
  // Don't use use Serial.begin --> Assume serial is running on 115200 and use the printf function its MUCH faster
  ESP_LOGI(BASE_TAG, "ESP-STARTING");
  setup_io();

  // TODO: MAYBE?? QUICK LED SHOW WHEN STEERING WHEEL FIRST TURNED ON (this can be done much later)

  printf("\n\t\tInitializing CAN\n\n");
  
  // Call the ESP-IDF specific startup procedure
  app_main();

}

void loop(){
  // do nothing in this loop, the FreeRTOS is managing task scheduling above
}
