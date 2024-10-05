// Define the pin for the IR sensor and LED
const int irSensorPin = 2;  // Pin connected to the IR sensor's output
const int ledPin = 13;      // Pin connected to the LED

void setup() {
  // Initialize the serial communication for debugging
  Serial.begin(9600);

  // Set the IR sensor pin as input
  pinMode(irSensorPin, INPUT);

  // Set the LED pin as output
  pinMode(ledPin, OUTPUT);

  // Initially turn off the LED
  digitalWrite(ledPin, LOW);
}

void loop() {
  // Read the value from the IR sensor
  int irSensorValue = digitalRead(irSensorPin);

  // Print the IR sensor value to the Serial Monitor
  Serial.println(irSensorValue);

  // Check if the IR sensor detects an object
  if (irSensorValue == HIGH) {
    // Object detected, turn on the LED
    Serial.println("The button is unpressed");
  } else {
    // No object detected, turn off the LED
    Serial.println("the button is pressed");
  }

  // Add a small delay to avoid flooding the Serial Monitor
  delay(100);
}
