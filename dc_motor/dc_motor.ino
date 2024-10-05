const int switchPin=4;
int switchState=1;

void setup() {
  // put your setup code here, to run once:
pinMode(switchPin,INPUT);
pinMode(2,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
switchState=digitalRead(switchPin);
if(switchPin==HIGH)
{
  digitalWrite(2,HIGH);
  }
  else
  {
   digitalWrite(2,LOW);
    }
}
