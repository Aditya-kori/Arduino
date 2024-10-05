// Define pins for the ultrasonic sensor
#define TRIG_PIN 9
#define ECHO_PIN 10

void setup() {
  // Start serial communication at 9600 baud rate
  Serial.begin(9600);

  // Initialize the TRIG_PIN as an output
  pinMode(TRIG_PIN, OUTPUT);
  
  // Initialize the ECHO_PIN as an input
  pinMode(ECHO_PIN, INPUT);

  Serial.println("Ultrasonic Sensor Initialized");
}

void loop() {
  // Send a 10 microsecond pulse to the TRIG_PIN
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Read the ECHO_PIN, pulseIn() returns the duration (in microseconds) of the pulse
  long duration = pulseIn(ECHO_PIN, HIGH);

  // Calculate the distance in centimeters
  long distance = duration * 0.034 / 2;

  // Print the distance to the Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Small delay before the next reading
  delay(100);
}
