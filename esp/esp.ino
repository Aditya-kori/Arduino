#include <SoftwareSerial.h>
#include <stdlib.h>

SoftwareSerial ESP8266(2, 3); // RX, TX
unsigned char check_connection = 0;
unsigned char times_check = 0;

void setup() {
  Serial.begin(115200);
  ESP8266.begin(115200);
  ESP8266.print("***VER");
  delay(2000);

  ESP8266.println("AT+RST"); // Reset the module
  delay(2000); // Wait for the module to reset
  
  ESP8266.println("AT+GMR"); // Get firmware version
  delay(1000);
  ESP8266.println("AT+CWMODE=3"); // Set WiFi mode to STA
  delay(1000);
}

void loop() {
  Serial.println("Connecting to Wifi");
  
  while (check_connection == 0) {
    Serial.print(".");
    ESP8266.println("AT+CWJAP=\"Galaxy M21D71E\",\"12345678\"");
    ESP8266.setTimeout(10000); // Set timeout for 10 seconds
    
    if (ESP8266.find("WIFI CONNECTED")) {
      Serial.println("WIFI CONNECTED");
      check_connection = 1; // Exit loop on successful connection
    } else {
      times_check++;
      if (times_check > 3) {
        times_check = 0;
        Serial.println("Trying to Reconnect");
        ESP8266.println("AT+CWQAP"); // Disconnect from any network
        delay(1000);
      }
    }
  }
  
  // After connecting, you might want to add additional code here
  while (1) {
    // Infinite loop to prevent further reconnection attempts
  }
}
