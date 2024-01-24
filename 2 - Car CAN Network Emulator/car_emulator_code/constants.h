
//Example Configurations
#define RUN_PRE_CHECK           true
#define NO_OF_MSGS              100
#define NO_OF_ITERS             3
#define TX_GPIO_NUM             10
#define RX_GPIO_NUM             9

#define IO_TSK_PRIO             8       //IO task priority
#define TX_TASK_PRIO            8       //Sending task priority
#define RX_F_TASK_PRIO          9       //Receiving task priority
#define RX_TASK_PRIO            9       //Receiving task priority
#define CTRL_TSK_PRIO           10      //Control task priority

#define EXAMPLE_TAG             "TWAI Self Test"

/* --------------------- Outputs ------------------ */

  // STATUS LEDS
    #define LED_1_PIN 32   // Location:? Color: Blue
    #define LED_2_PIN 33   // Location:? Color: Green
    #define LED_3_PIN 25   // Location:? Color: Blue
    #define LED_4_PIN 26   // Location:? Color: Yellow
    #define LED_5_PIN 27   // Location:? Color: Yellow

    #define LED_BUILTIN 2  // 


/* --------------------- Special Messages ------------------ */
  
  // CAN Addresses
    #define LEDS_CONTROL_ADDR 0x7F9
    #define MSG_ID            0x555   //11 bit standard format ID
    
  // Masks
    #define LED_1_MASK (1 << 0)   // 00000001
    #define LED_2_MASK (1 << 1)   // 00000010
    #define LED_3_MASK (1 << 2)   // 00000100
    #define LED_4_MASK (1 << 3)   // 00001000
    #define LED_5_MASK (1 << 4)   // 00010000