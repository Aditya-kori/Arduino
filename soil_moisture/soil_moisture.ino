// Define the analog pin connected to the soil moisture sensor
const int sensorPin = 3;

// Variables to store sensor readings
int sensorValue = 0;
int sensorMin = 0; // Adjust these values based on calibration
int sensorMax = 1023;

void setup() {
  // Initialize serial communication at 9600 baud
  Serial.begin(9600);
  
  // Optionally, calibrate the sensor
  // Uncomment these lines and observe the min and max values on the Serial Monitor
  // to determine the dry and wet values
  // sensorMin = analogRead(sensorPin);
  // sensorMax = analogRead(sensorPin);
}

void loop() {
  // Read the value from the soil moisture sensor
  sensorValue = analogRead(sensorPin);
  
  // Convert the analog reading (0-1023) to a percentage (0-100%)
  int moisturePercent = map(sensorValue, sensorMin, sensorMax, 0, 100);
  
  // Print the sensor value and moisture percentage to the Serial Monitor
  Serial.print("Sensor Value: ");
  Serial.print(sensorValue);
 
  
  // Wait for a second before taking another reading
  delay(1000);
}
