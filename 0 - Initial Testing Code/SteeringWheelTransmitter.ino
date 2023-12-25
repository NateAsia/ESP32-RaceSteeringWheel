#define LED1  9
#define LED2  10
#define LED3  11
#define LED4  8
#define button 12

boolean button_state = false;

boolean led_state = false;

#define joystick_x A0 
#define joystick_y A1
#define joystick_click 13
int xval = 0;
int yval = 0;

int xled = 0;
int yled = 0;

//serial communication map
//a -> normal button clicked
char button_out = 'a';
//j -> joystick button clicked
char joystick_click_out = 'j';

//joystick values: 0 to 1023


void setup() {
  Serial.begin(9600);

  pinMode(LED1, OUTPUT);

  pinMode(joystick_x, INPUT);
  pinMode(joystick_y, INPUT);
  pinMode(joystick_click, INPUT);

  pinMode(button, INPUT);

}

void loop() {

  read_button();

  read_joystick();

  delay(2);
}

void read_button() {
  //button is pin to read
	
  //read state of button
  if(button_state != digitalRead(button) && digitalRead(button) == true){
    digitalWrite(LED1, HIGH);
    Serial.write(button_out);
    button_state = true;
  }
  else if (digitalRead(button) == false) {
    digitalWrite(LED1, LOW);
    button_state = false;
  }
}

void read_joystick() {
  //read x, y values
  xval = analogRead(joystick_x);
  yval = analogRead(joystick_y);
  int xled = xval / 5;
  int yled = yval / 5;
  analogWrite(LED2, xled);
  analogWrite(LED3, yled);

  Serial.write("x");
  Serial.write(xled);

  Serial.write("y");
  Serial.write(yled);

  //read joystick pushbutton, only if off
  if(led_state != !digitalRead(joystick_click) && !digitalRead(joystick_click) == true){
    digitalWrite(LED4, HIGH);
    Serial.write(joystick_click_out);
    led_state = true;
  }
  else if (!digitalRead(joystick_click) == false) {
    digitalWrite(LED1, LOW);
    led_state = false;
  }
}
