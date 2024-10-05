#include <SoftwareSerial.h>
#include <DHT.h>

#define DHTPIN 7     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11

SoftwareSerial bluetooth(3,2); // RX, TX
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);   // Serial monitor
  bluetooth.begin(9600); // Bluetooth serial port
  
  dht.begin();
}

void loop() {
  // Reading temperature or humidity takes about 250 milliseconds!
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Send humidity and temperature data over Bluetooth
  bluetooth.print("Humidity: ");
  bluetooth.print(humidity);
  bluetooth.print("%  Temperature: ");
  bluetooth.print(temperature);
  bluetooth.println("°C");

  // Wait a few seconds between measurements.
  delay(2000);
}
