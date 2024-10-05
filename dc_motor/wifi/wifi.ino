#include<SoftwareSerial.h>
#include<stdlib.h>
SoftwareSerial ESP8266(2,3);
unsigned char check_connection=0;
unsigned char times_check=0;
void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
ESP8266.begin(115200);
ESP8266.print("***VER:");
delay(2000);
ESP8266.println("AT+RST");
delay(1000);
ESP8266.println("AT+GMR");
delay(1000);
ESP8266.println("AT+CWMODE");
delay(1000);

}

void loop() {
  Serial.println("connect to wifi");
  while(check_connection==0)
  {
    Serial.print(".");
    ESP8266.print("AT+CWJAP=\"Galaxy M21D71E\",\"kushalkori\"\r\n");  
    ESP8266.setTimeout(5000);
    if(ESP8266.find("WIFI CONNECTED\r\n")==1)
    {
      Serial.println("WIFI CONNECTED");
      break;
    }
    times_check++;
    if(times_check>3)
    {
      times_check=0;
      Serial.println("trying to recconect__");
  }
    }
  while(1);
  

}
