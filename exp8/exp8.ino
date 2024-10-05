#include<SoftwareSerial.h>
SoftwareSerial mySerial(3,2);
const int buttonPin=6;
int buttonState=1;
void setup() {
  // put your setup code here, to run once:
pinMode(buttonPin,INPUT);
pinMode(LED_BUILTIN,OUTPUT);
mySerial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
buttonState=digitalRead(buttonPin);
if(buttonState==0)
{digitalWrite(LED_BUILTIN,HIGH);
mySerial.println("Intruder detected & LED is turned ON");
}
else
{digitalWrite(LED_BUILTIN,LOW);
mySerial.println("Intruder not detected & LED is turned OFF");
}
delay(3000);
}
