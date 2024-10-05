#include<SoftwareSerial.h>
const int relayPin=13;
String message="";
SoftwareSerial mySerial(3,2);
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
pinMode(relayPin, OUTPUT);
digitalWrite(relayPin, HIGH);
mySerial.begin(9600);
Serial.println("Initializing...");
Serial.println("The device started, now you can pair it with bluetooth!");


}

void loop() {
  if (mySerial.available()){
    char incomingChar = mySerial.read();
    if (incomingChar != '\n'){
      message += String(incomingChar);
    }
    else{
      message = "";
    }
    Serial.write(incomingChar);  
  }
  if (message == "on"){
    digitalWrite(relayPin, LOW);
  }
  else if (message == "off"){
    digitalWrite(relayPin, HIGH);
  }
  delay(20);

  // put your main code here, to run repeatedly:

}
