#define LED1  16
#define LED2  5
#define LED3  4

//use generic ESP8266 Module
int led1_counter = 0;
int led2_counter = 0;
int x_led = 0;
int y_led = 0;


void setup() {
  Serial.begin(9600);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);

  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, HIGH);
  delay(100);
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
}

void loop() {
  if(led1_counter>0){
    led1_counter = led1_counter +1;
  }
  if(led2_counter>0){
    led2_counter = led2_counter +1;
  }

  if(Serial.available() > 0){
      char c = Serial.read();
    if(c == 'a' && led1_counter == 0){
      digitalWrite(LED1, HIGH);
      led1_counter=1;
    }
    else if(c == 'j' && led2_counter == 0){
      digitalWrite(LED2, HIGH);
      led2_counter=1;
    }
    else if(c == 'x') { //indicates that coordinates are being sent
      x_led = Serial.read();
    }
    else if(c == 'y') {
      y_led = Serial.read();
    }
  }

  if(led1_counter>500){
    led1_counter = 0;
    digitalWrite(LED1, LOW);
  }
  if(led2_counter>500){
    led2_counter = 0;
    digitalWrite(LED2, LOW);
  }

  analogWrite(LED2, x_led);
  analogWrite(LED3, y_led);
  
  delay(2);

}
