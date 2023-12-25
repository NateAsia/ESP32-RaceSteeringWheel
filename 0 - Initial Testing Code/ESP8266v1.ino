#define INPUT_PIN1 1
#define INPUT_PIN2 3
#define INPUT_PIN3 15
#define LED_OUTPUT 2

void button1(); 
void button2(); 
void button3(); 

void setup() 
{
  pinMode(INPUT_PIN1, INPUT_PULLUP);
  pinMode(INPUT_PIN2, INPUT_PULLUP);
  pinMode(INPUT_PIN3, INPUT_PULLUP);
  pinMode(LED_OUTPUT, OUTPUT);
  // put your setup code here, to run once:
  Serial.begin( 9600 );
}

void loop() {
  Serial.println(digitalRead(INPUT_PIN1));
  // put your main code here, to run repeatedly:
  if (digitalRead(INPUT_PIN1) == LOW)
  {
    button1(); 
  }
  if(digitalRead(INPUT_PIN2) == LOW)
  {
    button2();
  }
  if(digitalRead(INPUT_PIN3) == LOW)
  {
    button3();
  }

  
  delay(1); 
}

void button1()
{
  digitalWrite(LED_OUTPUT, HIGH);
  delay(800); 
  digitalWrite(LED_OUTPUT, LOW);
  delay(800); 
}

void button2()
{
  digitalWrite(LED_OUTPUT, HIGH);
  delay(400); 
  digitalWrite(LED_OUTPUT, LOW);
  delay(400); 
}

void button3()
{
  digitalWrite(LED_OUTPUT, HIGH);
  delay(100); 
  digitalWrite(LED_OUTPUT, LOW);
  delay(100); 
}

