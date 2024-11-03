/* 
  ******************** INPUT TASK FILE ********************

  - Please for all functions related to inputs and states

 */


/* --------------------- Variables ------------------ */

    bool      button_1  = false;     
    bool      button_2  = false;     
    bool      button_3  = false;     
    
    bool      btn_rear_1   = false;     
    bool      btn_rear_2   = false;     

    uint8_t   front_btn_status    = 0b00000000;   
    uint8_t   rear_btn_status     = 0b00000000; 
    uint8_t   special_btn_status  = 0b00000000;  
    uint8_t   button_status       = 0b00000000;  
    
    uint8_t   modes_status        = 0b00000000; 
    uint8_t   screen_btns         = 0b00000000;

    ROTARY_BTN left_rty_btn   = { 
      .state            = false,
      .last_state       = false, 
      .disable          = true,
      .time_hold_start  = 0, 
      .mode             = 0, 
      .num_of_modes     = 4};
    ROTARY_BTN right_rty_btn  = {
      .state            = false,
      .last_state       = false, 
      .disable          = true,
      .time_hold_start  = 0, 
      .mode             = 0, 
      .num_of_modes     = 4};
    bool in_rty_update_mode   = false;
    bool dual_hold            = false;
    bool holding_too_long     = false;
    long hold_start_time      = 0;
    long last_btn_press       = 0;
    ROTARY_BTN *r_list[ROTARY_BTN_LIST_SIZE] = {&left_rty_btn, &right_rty_btn};
  
    uint16_t  pot_1     = 0x0000;      // mV of POT_1_PIN
    uint16_t  pot_2     = 0x0000;      // mV of POT_2_PIN

    uint16_t  pot_v     = 0x0000;      // MAX of pot 1 & 2
    uint8_t   pot_v_h   = 0x00;
    uint8_t   pot_v_l   = 0x00;

    bool      special_1 = false;
    bool      special_2 = false;


/* --------------------- Functions ------------------ */

//
void setup_io(){
  
  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(BUTTON_1_PIN, INPUT_PULLUP);  
  pinMode(BUTTON_2_PIN, INPUT_PULLUP);
  pinMode(BUTTON_3_PIN, INPUT_PULLUP);

  pinMode(BTN_REAR_1, INPUT_PULLUP);
  pinMode(BTN_REAR_2, INPUT_PULLUP);

  pinMode(ROTARY_BTN_1_PIN, INPUT_PULLUP);
  pinMode(ROTARY_BTN_2_PIN, INPUT_PULLUP);

  pinMode(POT_1_PIN, INPUT);
  pinMode(POT_2_PIN, INPUT);

  EEPROM.begin(EEPROM_SIZE); // Setup Flash Memory

  left_rty_btn.mode   = EEPROM.readByte(RTY_1_EE_LOCATION);  // Read Last mode of rty_position from memory on boot
  right_rty_btn.mode  = EEPROM.readByte(RTY_2_EE_LOCATION);  // Read Last mode of rty_position from memory on boot

}

void rotary_update_mode_toggle(){
  in_rty_update_mode = !in_rty_update_mode;
  holding_too_long = true;
  if(!in_rty_update_mode){     // When we exit rotary update mode, update the EEPROM
      EEPROM.writeByte(RTY_1_EE_LOCATION, left_rty_btn.mode);
      EEPROM.writeByte(RTY_2_EE_LOCATION, right_rty_btn.mode);
      EEPROM.commit();
  }
  else{
    left_rty_btn.disable = true;
    right_rty_btn.disable = true;
  }
}

//
void rotary_input_check(){

  left_rty_btn.state  = !digitalRead(ROTARY_BTN_1_PIN);
  right_rty_btn.state = !digitalRead(ROTARY_BTN_2_PIN);

  if(left_rty_btn.state|right_rty_btn.state){
    last_btn_press = millis();
  }

  // Entering and Exiting Rotary Update Mode
    if(left_rty_btn.state & right_rty_btn.state) {
      if(!dual_hold){
        dual_hold = true;
        hold_start_time = millis();
      }
      else{
        if(!holding_too_long && millis() - hold_start_time >= ROTARY_WAIT_TIME){
          rotary_update_mode_toggle();
        }
        return;  // if holding both, do not continue reading for mode swichtes
      }
    }
    else{
      dual_hold = false;
      holding_too_long = false;
    }
    
    if(in_rty_update_mode & (millis() - last_btn_press > ROTARY_NO_INPUT_TIME)){
      rotary_update_mode_toggle();
    }

  // When in Rotary Update Mode, update external modes with a button press (rising edge)
    if (in_rty_update_mode){
      for(int i = 0; i < ROTARY_BTN_LIST_SIZE; i++){

        // Start with debouncing the button
          if (r_list[i]->last_state != r_list[i]->state){
            r_list[i]->time_hold_start = millis();
            r_list[i]->last_state = r_list[i]->state; 

            if (!r_list[i]->state){
              r_list[i]->disable = false;
            }

          }
        
        // Once Debounced, add to the mode counter if the button state is high (pressed) and not disabled (already pressed)
        if ((r_list[i]->state) & (!r_list[i]->disable) & (millis() - r_list[i]->time_hold_start > RTY_BTN_DEBOUNCE_LONG)){
          r_list[i]->mode += r_list[i]->state;
          r_list[i]->disable = true;

          if (r_list[i]->mode >= r_list[i]->num_of_modes){
            r_list[i]->mode = 0;
          }
        }
      }
    }
}

//
void pot_input_check(){
  pot_1     = analogReadMilliVolts(POT_1_PIN);
  pot_2     = analogReadMilliVolts(POT_2_PIN); 

  pot_v     = max(pot_1, pot_2); 
  pot_v_h   = (pot_v & 0xFF00) >> 8;        // High Bits
  pot_v_l   = (pot_v & 0x00FF);             // Low Bits 
}

//
void custom_states(){
  
  special_1 = 
      (pot_1>2600) 
      & (pot_2>2600) 
      & left_rty_btn.state 
      & !in_rty_update_mode 
      & !dual_hold;           // :P

  special_2 = 
      (pot_1>2600) 
      & (pot_2>2600) 
      & right_rty_btn.state 
      & !in_rty_update_mode 
      & !dual_hold;           // :P

}

void screen_btn_update(){
  screen_btns = ((
        left_rty_btn.state 
        & !in_rty_update_mode 
        & !dual_hold) 
        << 7
      ) | ((
        right_rty_btn.state 
        & !in_rty_update_mode 
        & !dual_hold) 
        << 6
      );
}


// Thread to update the variables corresponding to steering wheel inputs (every 1ms)
static void update_input_statuses(void *arg){

  ESP_LOGI(BASE_TAG, "Update Inputs Thread Started");
  
  while(1){

      rotary_input_check();

      button_1  = !digitalRead(BUTTON_1_PIN);   // (!) since we are using a pullup network on buttons
      button_2  = !digitalRead(BUTTON_2_PIN);
      button_3  = !digitalRead(BUTTON_3_PIN);

      btn_rear_1 = !digitalRead(BTN_REAR_1);
      btn_rear_2 = !digitalRead(BTN_REAR_2); 

      pot_input_check();
      custom_states();
      screen_btn_update();

      front_btn_status = (button_1 << 7) | (button_2 << 6) | (button_3 << 5);  // the bottom two buttons are ignored (used for rty)
      rear_btn_status =  (btn_rear_1 << 4) | (btn_rear_2 << 3); 
      special_btn_status = (special_1 << 2) | (special_2 << 1);  
      
      
      button_status = front_btn_status | rear_btn_status | special_btn_status | in_rty_update_mode;

      modes_status = (left_rty_btn.mode << 4) | (right_rty_btn.mode);  


    vTaskDelay(pdMS_TO_TICKS(1));    // This will repeat every 1 ms
  }

  vTaskDelete(NULL);
}