#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_err.h"
#include "esp_log.h"
#include "driver/twai.h"



/* --------------------- Definitions and static variables ------------------ */

#include "constants.h"

static const twai_timing_config_t t_config = TWAI_TIMING_CONFIG_1MBITS();

//Filter all other IDs except MSG_ID
static const twai_filter_config_t f_config = {
                                              .acceptance_code = (MSG_ID << 21),
                                              .acceptance_mask = ~(TWAI_STD_ID_MASK << 21),
                                              .single_filter = true
                                             };

static const twai_filter_config_t f_config_full = TWAI_FILTER_CONFIG_ACCEPT_ALL();

//Set to NO_ACK mode due to self testing with single module
static const twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT( (gpio_num_t)TX_GPIO_NUM, (gpio_num_t)RX_GPIO_NUM, TWAI_MODE_NO_ACK);

static SemaphoreHandle_t tx_sem;
static SemaphoreHandle_t rx_sem;
static SemaphoreHandle_t rx_forever_sem;
static SemaphoreHandle_t ctrl_sem;
static SemaphoreHandle_t done_sem;

bool led1 = false;
bool led2 = false;
bool led3 = false;
bool led4 = false;
bool led5 = false;

twai_message_t message;

/* --------------------------- Tasks and Functions -------------------------- */

static void twai_transmit_task(void *arg){
    twai_message_t tx_msg = {.self = 1, .identifier = MSG_ID, .data_length_code = 1};
    for (int iter = 0; iter < NO_OF_ITERS; iter++) {
        xSemaphoreTake(tx_sem, portMAX_DELAY);
        for (int i = 0; i < NO_OF_MSGS; i++) {
            //Transmit messages using self reception request
            tx_msg.data[0] = i;
            ESP_ERROR_CHECK(twai_transmit(&tx_msg, portMAX_DELAY));
            vTaskDelay(pdMS_TO_TICKS(10));
        }
        // xSemaphoreGive(ctrl_sem);
    }
    vTaskDelete(NULL);
}

static void twai_receive_task(void *arg){
    twai_message_t rx_message;

    for (int iter = 0; iter < NO_OF_ITERS; iter++) {
        xSemaphoreTake(rx_sem, portMAX_DELAY);
        for (int i = 0; i < NO_OF_MSGS; i++) {
            //Receive message and print message data
            ESP_ERROR_CHECK(twai_receive(&rx_message, portMAX_DELAY));
            ESP_LOGI(EXAMPLE_TAG, "Msg received - Data = %d", rx_message.data[0]);
        }
        //Indicate to control task all messages received for this iteration
        xSemaphoreGive(ctrl_sem);
    }
    vTaskDelete(NULL);
}

static void twai_receive_forever_task(void *arg)
{
    xSemaphoreTake(rx_forever_sem, portMAX_DELAY);
    printf("Recieve Forever Loop\n");

    while(1){

        if (twai_receive(&message, portMAX_DELAY) == ESP_OK); 
        else {
            printf("\nFailed to receive message\n");
            continue;
        }

        digitalWrite(LED_BUILTIN, HIGH);
        // Process received message
        if (message.extd) {
            printf("\nExtended Format\tID: 0x%.08x\t", (int)message.identifier);
        } else {
            printf("\nStandard Format\tID: 0x%.03x\t", (int)message.identifier);
        }
        if (!(message.rtr)) {
            printf("Data (%d) (hex): ", (int)message.data_length_code);
            for (int i = 0; i < message.data_length_code; i++) {
                printf("0x%.02x ", message.data[i]);
            }
        }

        if(message.identifier == LEDS_CONTROL_ADDR){
          printf("\t| x:%d y:%d \n", message.data[1], message.data[2]);
          uint8_t byte1 = message.data[0];
          led1 = byte1 & LED_1_MASK;
          led2 = byte1 & LED_2_MASK;
          led3 = byte1 & LED_3_MASK;
          led4 = byte1 & LED_4_MASK;
          led5 = byte1 & LED_5_MASK;
        }


        digitalWrite(LED_BUILTIN, LOW);
        vTaskDelay(1);  // This will allow the CPU to switch to another task :)
    }
    
    
}

static void twai_control_task(void *arg){
    xSemaphoreTake(ctrl_sem, portMAX_DELAY);
    ESP_ERROR_CHECK(twai_start());
    ESP_LOGI(EXAMPLE_TAG, "Driver started");

    for (int iter = 0; iter < NO_OF_ITERS; iter++) {
        xSemaphoreGive(rx_sem);
        xSemaphoreGive(tx_sem);
        xSemaphoreTake(ctrl_sem, portMAX_DELAY);    //Wait for TX and RX tasks to finish iteration
        vTaskDelay(pdMS_TO_TICKS(100));             //Delay then start next iteration
    }

    ESP_ERROR_CHECK(twai_stop());               //Stop the TWAI Driver
    ESP_LOGI(EXAMPLE_TAG, "Driver stopped");

    digitalWrite(LED_1_PIN, LOW);
    digitalWrite(LED_2_PIN, LOW);
    digitalWrite(LED_3_PIN, LOW);
    digitalWrite(LED_4_PIN, LOW);
    digitalWrite(LED_5_PIN, LOW);

    xSemaphoreGive(done_sem);
    vTaskDelete(NULL);
}

static void update_io(void *arg){
  while(1){
    digitalWrite(LED_1_PIN, led1);
    digitalWrite(LED_2_PIN, led2);
    digitalWrite(LED_3_PIN, led3);
    digitalWrite(LED_4_PIN, led4);
    digitalWrite(LED_5_PIN, led5);
    vTaskDelay(pdMS_TO_TICKS(10));
  }

  vTaskDelete(NULL);
}

void app_main(void)
{
    //Create tasks and synchronization primitives
    tx_sem = xSemaphoreCreateBinary();
    rx_sem = xSemaphoreCreateBinary();
    rx_forever_sem = xSemaphoreCreateBinary();
    ctrl_sem = xSemaphoreCreateBinary();
    done_sem = xSemaphoreCreateBinary();

    if(RUN_PRE_CHECK){
      xTaskCreatePinnedToCore(twai_control_task,        "TWAI_ctrl",      4096, NULL, CTRL_TSK_PRIO,  NULL, tskNO_AFFINITY);
      xTaskCreatePinnedToCore(twai_receive_task,        "TWAI_rx",        4096, NULL, RX_TASK_PRIO,   NULL, tskNO_AFFINITY);
      xTaskCreatePinnedToCore(twai_transmit_task,       "TWAI_tx",        4096, NULL, TX_TASK_PRIO,   NULL, tskNO_AFFINITY);

      //Install TWAI driver
      ESP_ERROR_CHECK(twai_driver_install(&g_config, &t_config, &f_config));
      ESP_LOGI(EXAMPLE_TAG, "Seld Detect Driver installed");

      //Start control task
      xSemaphoreGive(ctrl_sem);
      //Wait for all iterations and tasks to complete running
      xSemaphoreTake(done_sem, portMAX_DELAY);

      //Uninstall TWAI driver
      ESP_ERROR_CHECK(twai_driver_uninstall());
      ESP_LOGI(EXAMPLE_TAG, "Driver uninstalled");

      vSemaphoreDelete(tx_sem);
      vSemaphoreDelete(rx_sem); 
    }

    // Small Light Show :)
    vTaskDelay(pdMS_TO_TICKS(2000));
    digitalWrite(LED_1_PIN, LOW);
    vTaskDelay(pdMS_TO_TICKS(200));
    digitalWrite(LED_2_PIN, LOW);
    vTaskDelay(pdMS_TO_TICKS(200));
    digitalWrite(LED_3_PIN, LOW);
    vTaskDelay(pdMS_TO_TICKS(200));
    digitalWrite(LED_4_PIN, LOW);
    vTaskDelay(pdMS_TO_TICKS(200));
    digitalWrite(LED_5_PIN, LOW);
    vTaskDelay(pdMS_TO_TICKS(200));

    xTaskCreatePinnedToCore(twai_receive_forever_task,  "TWAI_rx_forever",  4096, NULL, RX_F_TASK_PRIO, NULL, tskNO_AFFINITY);
    xTaskCreatePinnedToCore(update_io,                  "IO_UPDATE",        4096, NULL, IO_TSK_PRIO,    NULL, tskNO_AFFINITY);

    ESP_ERROR_CHECK(twai_driver_install(&g_config, &t_config, &f_config_full));
    ESP_LOGI(EXAMPLE_TAG, "Full Driver installed");

    ESP_ERROR_CHECK(twai_start());
    ESP_LOGI(EXAMPLE_TAG, "Driver started");

    xSemaphoreGive(rx_forever_sem); // Start forever receiving
    xSemaphoreTake(done_sem, portMAX_DELAY);



    //Cleanup
    vSemaphoreDelete(rx_forever_sem);
    vSemaphoreDelete(ctrl_sem);
    vQueueDelete(done_sem);
}

void setup(){
  ESP_LOGI(EXAMPLE_TAG, "ESP-STARTING");

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  pinMode(LED_1_PIN, OUTPUT);
  pinMode(LED_2_PIN, OUTPUT);
  pinMode(LED_3_PIN, OUTPUT);
  pinMode(LED_4_PIN, OUTPUT);
  pinMode(LED_5_PIN, OUTPUT);

  digitalWrite(LED_1_PIN, HIGH);
  digitalWrite(LED_2_PIN, HIGH);
  digitalWrite(LED_3_PIN, HIGH);
  digitalWrite(LED_4_PIN, HIGH);
  digitalWrite(LED_5_PIN, HIGH);


  printf("start\n");

  app_main();
}

void loop(){
  // Do nothing FreeRTOS Managing CPU :)

}
