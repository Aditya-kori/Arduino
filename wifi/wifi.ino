#include <SoftwareSerial.h>

// Define software serial pins for ESP8266
SoftwareSerial espSerial(2, 3); // RX, TX

// Replace with your network credentials
const char* ssid = "Galaxy M21D71E";
const char* password = "12345678";

// ThingSpeak Settings
const char* apiKey = "2429497";
const char* server = "api.thingspeak.com";

void setup() {
  Serial.begin(115200); // Initialize serial communication for debugging
  espSerial.begin(115200); // Initialize software serial for ESP8266
  
  connectWiFi();
}

void loop() {
  // Read soil moisture sensor
  int sensorValue = analogRead(A0); // Replace A0 with the pin you connected your sensor to
  Serial.print("Soil Moisture Sensor Value: ");
  Serial.println(sensorValue);
  
  // Map the sensor value to a percentage
  int soilMoisture = map(sensorValue, 1023, 0, 0, 100);
  Serial.print("Soil Moisture (%): ");
  Serial.println(soilMoisture);

  // Upload sensor data to ThingSpeak
  uploadToThingSpeak(soilMoisture);

  // Wait 15 seconds to update again
  delay(15000);
}

void connectWiFi() {
  sendData("***VER:", 2000, true);
  sendData("AT+GMR", 1000, true);
  sendData("AT+RST\r\n", 5000, true); // Reset the module
  sendData("AT+CWMODE=1\r\n", 1000, true); // Set mode to STA
  sendData("AT+CWJAP=\"" + String(ssid) + "\",\"" + String(password) + "\"\r\n", 10000, true); // Connect to Wi-Fi
}

void uploadToThingSpeak(int value) {
  sendData("AT+CIPSTART=\"TCP\",\"" + String(server) + "\",80\r\n", 1000, true);
  
  String getStr = "GET /update?api_key=" + String(apiKey) + "&field1=" + String(value) + "\r\n";
  sendData("AT+CIPSEND=" + String(getStr.length()) + "\r\n", 1000, true);
  sendData(getStr, 1000, true);
  sendData("AT+CIPCLOSE\r\n", 1000, true);
}

String sendData(String command, const int timeout, boolean debug) {
  String response = "";
  espSerial.print(command); // Send the command to the ESP8266
  
  long int time = millis();
  while ( (time + timeout) > millis()) {
    while (espSerial.available()) {
      char c = espSerial.read();
      response += c;
    }
  }
  
  if (debug) {
    Serial.print(response);
  }
  
  return response;
}
