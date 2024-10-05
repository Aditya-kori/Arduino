const int buttonPin=2;
int buttonState=0;
void setup() {
  // put your setup code here, to run once:
pinMode(buttonPin,INPUT);
pinMode(LED_BUILTIN,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
buttonState=digitalRead(buttonPin);
if(buttonState==HIGH)
{
  digitalWrite(LED_BUILTIN,LOW);
  }
  else
  {
    digitalWrite(LED_BUILTIN,HIGH);
  }
}
