
#define VRX_PIN  A0 // Arduino pin connected to VRX pin
#define VRY_PIN  A1 // Arduino pin connected to VRY pin
#define LED_PIN  3
#define LED_PIN2  9
#define LED_PIN3  7
#define click  13

int xval = 0;
int yval = 0;

boolean ledOn = false;
int counter = 0;

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  pinMode(LED_PIN2, OUTPUT);
  pinMode(LED_PIN3, OUTPUT);
  pinMode(VRX_PIN, INPUT);
  pinMode(VRY_PIN, INPUT);
  pinMode(click, INPUT);
}

void loop() {
  xval = analogRead(VRX_PIN);
  yval = analogRead(VRY_PIN);
  int xled = xval / 5;
  int yled = yval / 5;
  analogWrite(LED_PIN, xled);
  analogWrite(LED_PIN2, yled);

  if(!digitalRead(click)){
    digitalWrite(LED_PIN3, HIGH);
    ledOn = true;
  }

  if(ledOn){
    counter++;
  }

  if(counter>500){
    counter = 0;
    ledOn = false;
    digitalWrite(LED_PIN3, LOW);
  }

  delay(2);



}
