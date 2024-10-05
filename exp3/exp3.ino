const int BUTTON_PIN=7;
const int RELAY_PIN=3;
void setup() {
  // put your setup code here, to run once:
pinMode(BUTTON_PIN,INPUT_PULLUP);
pinMode(RELAY_PIN,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
int buttonState=digitalRead(BUTTON_PIN);
if(buttonState==LOW)
{digitalWrite(RELAY_PIN,HIGH);
}
else
if(buttonState==HIGH)
{Serial.println("The buttton is unpressed");
digitalWrite(RELAY_PIN,LOW);
}
}
