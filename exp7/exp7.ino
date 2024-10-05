#include<SoftwareSerial.h>
#include<dht.h>
#define DHT11_PIN 7
dht DHT;
SoftwareSerial mySerial(3,2);
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
mySerial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
int chk=DHT.read11(DHT11_PIN);
if(isnan(DHT.humidity)||isnan(DHT.temperature))
{
 Serial.println("Failed to read from dht sensor!");
return;
}
Serial.print("Humidity=");
Serial.print(DHT.humidity);

Serial.print("Temperature=");
Serial.println(DHT.temperature);
Serial.print("c");
mySerial.print("Humidity=");
mySerial.println(DHT.humidity);

mySerial.print("Temperature=");
mySerial.print(DHT.temperature);
mySerial.println("c");
delay(3000);
}
