// Define pins for the accelerometer
const int xPin = A0;  // Pin connected to X output
const int yPin = A1;  // Pin connected to Y output
const int zPin = A2;  // Pin connected to Z output

// Define thresholds for voltage ranges (adjust as needed)
const float lowThreshold = 2.5;   // Below this voltage is considered "Low"
const float mediumThreshold = 3.0; // Between lowThreshold and this is "Medium"
const float highThreshold = 3.4;  // Above this voltage is considered "High"

void setup() {
  // Start serial communication
  Serial.begin(9600);
  Serial.println("GY-61 (ADXL335) Accelerometer Test");
}

void loop() {
  // Read the analog values from the accelerometer
  int xValue = analogRead(xPin);
  int yValue = analogRead(yPin);
  int zValue = analogRead(zPin);

  // Convert the analog readings to voltage
  float xVoltage = xValue * (5.0 / 1023.0);
  float yVoltage = yValue * (5.0 / 1023.0);
  float zVoltage = zValue * (5.0 / 1023.0);
  float accelMagnitude = sqrt(xVoltage * xVoltage + yVoltage * yVoltage + zVoltage * zVoltage);
  Serial.println(accelMagnitude);
  // Determine the risk level and print the corresponding message
  if (accelMagnitude <= lowThreshold) {
    Serial.println("Landslide Risk Level: Low");
  } else if (accelMagnitude > lowThreshold && accelMagnitude <= mediumThreshold) {
    Serial.println("Landslide Risk Level: Middle");
  } else {
    Serial.println("Landslide Risk Level: High");
  }

  // Categorize and print the voltage values
  Serial.print("X Voltage: ");
  Serial.print(xVoltage);
  Serial.print(" V, Status: ");
  Serial.println(getStatus(xVoltage));

  Serial.print("Y Voltage: ");
  Serial.print(yVoltage);
  Serial.print(" V, Status: ");
  Serial.println(getStatus(yVoltage));

  Serial.print("Z Voltage: ");
  Serial.print(zVoltage);
  Serial.print(" V, Status: ");
  Serial.println(getStatus(zVoltage));

  Serial.println(); // Newline for better readability

  // Add a delay before the next reading
  delay(500);
}

// Function to determine the status based on voltage
String getStatus(float voltage) {
  if (voltage < lowThreshold) {
    return "Low";
  } else if (voltage < mediumThreshold) {
    return "Medium";
  } else if (voltage < highThreshold) {
    return "High";
  } else {
    return "Over range";
  }
}
