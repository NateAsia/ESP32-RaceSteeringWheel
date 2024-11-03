/* 
  ******************** CAN TASK FILE ********************

  - Please for all functions related to twai/CAN

 */

// Variables
  static const twai_filter_config_t   f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();   // We can filter our CAN reception to specific addresses only (later)
  static const twai_timing_config_t   t_config = TWAI_TIMING_CONFIG_1MBITS();       // CAN SPEED (must change to 1000Kbits to work with our MoTeC System)         SPEED... I AM SPEED
  static const twai_general_config_t  g_config = TWAI_GENERAL_CONFIG_DEFAULT((gpio_num_t)TX_GPIO_NUM, (gpio_num_t)RX_GPIO_NUM, TWAI_MODE_NO_ACK);
  static SemaphoreHandle_t tx_sem;
  
  twai_message_t message;         // CAN Message  LL;L;;;/Struct    


// Functions

static void twai_transmit_task(void *arg){
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
                                    .identifier = 0x661, 
                                    .data_length_code = 8,
                                    .data = {
                                      button_status, 
                                      modes_status,
                                      screen_btns,
                                      pot_v_h, 
                                      pot_v_l, 
                                      0,
                                      0,
                                      0
                                      } 
                                  };
        
        Serial.print("BTNS:");
        Serial.print(button_1);
        Serial.print("@");
        Serial.print(button_2);
        Serial.print("@");
        Serial.print(left_rty_btn.state);
        Serial.print("@");

        Serial.print(button_3);
        Serial.print("@");
        Serial.print(right_rty_btn.state);
        Serial.print("@");
        
        Serial.print(btn_rear_1);
        Serial.print("@");
        Serial.print(btn_rear_2);

        Serial.print(":POTS:");
        Serial.print(pot_1);
        Serial.print("@");
        Serial.print(pot_2);
        Serial.print("@");
        Serial.print(pot_v);

        Serial.print(":MODES:");
        Serial.print(in_rty_update_mode);
        Serial.print("@");
        Serial.print(dual_hold);
        Serial.print("@");
        Serial.print(left_rty_btn.mode);
        Serial.print("@");
        Serial.print(right_rty_btn.mode);
        Serial.print("@");
        Serial.print(special_1);
        Serial.print("@");
        Serial.print(special_2);

        Serial.print("\n");


        ESP_ERROR_CHECK(twai_transmit(&message1 , portMAX_DELAY));  // Force the ESP to restart if there is a transmitted error

      digitalWrite(LED_BUILTIN, LOW);
      vTaskDelay(pdMS_TO_TICKS(10));    // Send updates @ 100 Hz
    }

    xSemaphoreGive(tx_sem);
    vTaskDelete(NULL);
}