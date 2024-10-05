#include <Wire.h>
#include <MPU6050.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

MPU6050 mpu;

int a = 0;
int b = 0;
int c = 0;
int HIGH_M = 0, MIDDLE_M = 0, LOW_M = 0;
int HIGH_S = 0, MIDDLE_S = 0, LOW_S = 0;
int HIGH_R = 0, MIDDLE_R = 0, LOW_R = 0;

// Define the analog pin for the soil moisture sensor
#define soilMoisturePin A0
// Define the analog pin for the rain sensor
#define rainSensorPin A1
// Define pins for Ultrasonic Sensors and LEDs
#define TRIG_PIN_1 1
#define ECHO_PIN_1 2
#define TRIG_PIN_2 3
#define ECHO_PIN_2 4
#define LED_R_1 5
#define LED_G_1 6
#define LED_R_2 7
#define LED_G_2 8
// Define pins of transmitter
// NRF24L01 pins
#define CE_PIN 9
#define CSN_PIN 10

// Initialize the NRF24L01 radio
RF24 radio(CE_PIN, CSN_PIN);

// Address through which the two modules communicate
const byte address[6] = "00001";

// Define the threshold distance in centimeters
#define THRESHOLD_DISTANCE 30

// Define thresholds for accelerometer readings (these thresholds need to be tuned based on your application)
const float lowThreshold = 1.05; // Low risk threshold for acceleration in g
const float middleThreshold = 2.4; // Middle risk threshold for acceleration in g

// Define thresholds for rain levels in volts
const float lowThresholdMin = 0.0;
const float lowThresholdMax = 2.25;
const float middleThresholdMin = 1.75;
const float middleThresholdMax = 2.75;
const float highThresholdMin = 2.25;
const float highThresholdMax = 5.0;

// Define the thresholds for moisture levels
const int lowThreshold_s = 300;
const int middleThreshold_s = 600;

void setup() {
  // Start serial communication at 115200 baud rate
  Serial.begin(115200);

  // Initialize MPU6050
  Wire.begin();
  mpu.initialize();

  // Check if MPU6050 is connected successfully
  if (!mpu.testConnection()) {
    Serial.println("MPU6050 connection failed");
    while (1);
  }

  Serial.println("MPU6050 connection successful");

  // Initialize the rain sensor pin as an input
  pinMode(rainSensorPin, INPUT);
  // Initialize the soil moisture sensor pin as an input
  pinMode(soilMoisturePin, INPUT);

  // Initialize pins for Ultrasonic Sensors
  pinMode(TRIG_PIN_1, OUTPUT);
  pinMode(ECHO_PIN_1, INPUT);
  pinMode(TRIG_PIN_2, OUTPUT);
  pinMode(ECHO_PIN_2, INPUT);

  // Initialize pins for LEDs
  pinMode(LED_R_1, OUTPUT);
  pinMode(LED_R_2, OUTPUT);
  pinMode(LED_G_1, OUTPUT);
  pinMode(LED_G_2, OUTPUT);

  // Turn off LEDs at start
  digitalWrite(LED_R_1, LOW);
  digitalWrite(LED_R_2, LOW);
  digitalWrite(LED_G_1, HIGH);
  digitalWrite(LED_G_2, HIGH);

  // Initialize the radio
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_LOW);
  radio.stopListening();

   Serial.println("Transmitter initialized");
}

void loop() {
  // MPU-6050 returns the values as LOW_M, MIDDLE_M, HIGH_M
  // Read accelerometer values
  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);

  // Convert to g (assuming a full-scale range of ±2g)
  float accelX = ax / 16384.0;
  float accelY = ay / 16384.0;
  float accelZ = az / 16384.0;

  // Calculate the magnitude of the acceleration vector
  float accelMagnitude = sqrt(accelX * accelX + accelY * accelY + accelZ * accelZ);

  // Print accelerometer values
  Serial.print("AccelX: "); Serial.print(accelX);
  Serial.print(" AccelY: "); Serial.print(accelY);
  Serial.print(" AccelZ: "); Serial.print(accelZ);
  Serial.print(" Magnitude: "); Serial.println(accelMagnitude);

  // Determine the risk level and print the corresponding message
  LOW_M = 0; MIDDLE_M = 0; HIGH_M = 0; // Reset values
  if (accelMagnitude <= lowThreshold) {
    LOW_M = 1;
    a = 1;
  } else if (accelMagnitude > lowThreshold && accelMagnitude <= middleThreshold) {
    MIDDLE_M = 1;
    a = 2;
  } else {
    HIGH_M = 1;
    a = 3;
  }

  // Wait for 1 second before taking another reading
  delay(1000);

  // Rain sensor returns LOW_R, MIDDLE_R, HIGH_R
  // Read the analog value from the rain sensor
  int rainSensorValue = analogRead(rainSensorPin);

  // Convert the analog value to voltage (assuming a 5V reference)
  float voltage = rainSensorValue * (5.0 / 1023.0);

  // Print the rain sensor value and voltage
  Serial.print("Rain Sensor Value: ");
  Serial.print(rainSensorValue);
  Serial.print(" Voltage: ");
  Serial.println(voltage);

  // Determine the rain level and print the corresponding message
  LOW_R = 0; MIDDLE_R = 0; HIGH_R = 0; // Reset values
  if (voltage >= lowThresholdMin && voltage <= lowThresholdMax) {
    LOW_R = 1;
    b = 1;
  } else if (voltage >= middleThresholdMin && voltage <= middleThresholdMax) {
    MIDDLE_R = 1;
    b = 2;
  } else if (voltage >= highThresholdMin && voltage <= highThresholdMax) {
    HIGH_R = 1;
    b = 3;
  } else {
    Serial.println("Rain Level: Out of Range");
  }

  // Wait for 1 second before taking another reading
  delay(1000);

  // Soil moisture sensor returns the values as LOW_S, MIDDLE_S, HIGH_S
  // Read the analog value from the soil moisture sensor
  int soilMoistureValue = analogRead(soilMoisturePin);

  // Print the soil moisture value
  Serial.print("Soil Moisture Value: ");
  Serial.println(soilMoistureValue);

  // Determine the moisture level and print the corresponding message
  LOW_S = 0; MIDDLE_S = 0; HIGH_S = 0; // Reset values
  if (soilMoistureValue >= 0 && soilMoistureValue <= lowThreshold_s) {
    LOW_S = 1;
    c = 1;
  } else if (soilMoistureValue > lowThreshold_s && soilMoistureValue <= middleThreshold_s) {
    MIDDLE_S = 1;
    c = 2;
  } else {
    HIGH_S = 1;
    c = 3;
  }

  // Wait for 1 second before taking another reading
  delay(1000);

  // Read distance from both ultrasonic sensors
  int distance1 = getDistance(TRIG_PIN_1, ECHO_PIN_1);
  int distance2 = getDistance(TRIG_PIN_2, ECHO_PIN_2);

  // Print distances for debugging
  //Serial.print("Distance 1: ");
  //Serial.print(distance1);
  //Serial.print(" cm, Distance 2: ");
  //Serial.print(distance2);
  //Serial.println(" cm");

  // Check if a vehicle is detected on one side and control LEDs accordingly
  if (distance1 < THRESHOLD_DISTANCE) {
    // Vehicle detected on side 1, turn on LED 2 (stop light for side 2)
    digitalWrite(LED_R_2, HIGH);
    digitalWrite(LED_G_2, LOW);
  } else {
    // No vehicle detected on side 1, turn off LED 2
    digitalWrite(LED_R_2, LOW);
    digitalWrite(LED_G_2, HIGH);
  }

  if (distance2 < THRESHOLD_DISTANCE) {
    // Vehicle detected on side 2, turn on LED 1 (stop light for side 1)
    digitalWrite(LED_R_1, HIGH);
    digitalWrite(LED_G_1, LOW);
  } else {
    // No vehicle detected on side 2, turn off LED 1
    digitalWrite(LED_R_1, LOW);
    digitalWrite(LED_G_1, HIGH);
  }

  // Small delay to avoid flooding the serial monitor
  delay(100);

  // To send message to LCD
  if (a == 1) {
    const char text[] = "LOW";
  bool success = radio.write(&text, sizeof(text));
   Serial.println(text);
  } else if (a == 2) {
    const char text[] = "MIDDLE";
  bool success = radio.write(&text, sizeof(text));
    Serial.println(text);
  } else if (a == 3) {
    const char text[] = "HIGH";
  bool success = radio.write(&text, sizeof(text));
    Serial.println(text);
  }
  

  delay(100); // Small delay to debounce the buttons
}

int getDistance(int trigPin, int echoPin) {
  // Send a 10 microsecond pulse to the trigger pin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the echo pin and calculate the distance
  long duration = pulseIn(echoPin, HIGH);
  int distance = duration * 0.034 / 2;

  // Return the distance if it's within a valid range
  if (distance >= 2 && distance <= 400) {
    return distance;
  } else {
    return -1; // Invalid distance
  }
}

void sendRiskMessage(const char* message) {
  // Send the message
  radio.write(message, strlen(message) + 1);

  // Print the message to the serial monitor for debugging
  Serial.println(message);
}
