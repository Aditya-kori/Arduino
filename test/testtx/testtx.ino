#include <Wire.h>
#include <MPU6050.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

MPU6050 mpu;

int a = 0;
int d = 0;
int b = 0;
int c = 0;
int HIGH_M = 0, MIDDLE_M = 0, LOW_M = 0;
int HIGH_G = 0, MIDDLE_G = 0, LOW_G = 0;
int HIGH_S = 0, MIDDLE_S = 0, LOW_S = 0;
int HIGH_R = 0, MIDDLE_R = 0, LOW_R = 0;

// Define the analog pin for the soil moisture sensor
#define soilMoisturePin A0
// Define the analog pin for the rain sensor
#define rainSensorPin A1
// Define pins for Ultrasonic Sensors and LEDs
#define IR_1 3
#define IR_2 4
#define LED_R_1 5
#define LED_G_1 6
#define LED_R_2 7
#define LED_G_2 8
// Define pins for Ultrasonic sensors
//#define TRIG_PIN_1 11
//#define ECHO_PIN_1 12
//#define TRIG_PIN_2 13
//#define ECHO_PIN_2 14
// Define pins of transmitter
// NRF24L01 pins
#define CE_PIN 9
#define CSN_PIN 10

// Initialize the NRF24L01 radio
RF24 radio(CE_PIN, CSN_PIN);

// Address through which the two modules communicate
const byte address[6] = "00001";

// Define the threshold distance in centimeters
//#define THRESHOLD_DISTANCE 30

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

  // Initialize pins for IR Sensors
  pinMode(IR_1, INPUT);
  pinMode(IR_2, INPUT);
  // Initialize pins for LEDs
  pinMode(LED_R_1, OUTPUT);
  pinMode(LED_R_2, OUTPUT);
  pinMode(LED_G_1, OUTPUT);
  pinMode(LED_G_2, OUTPUT);
  // Initialize pins for Ultrasonic Sensors
  //pinMode(TRIG_PIN_1, OUTPUT);
  //pinMode(ECHO_PIN_1, INPUT);
  //pinMode(TRIG_PIN_2, OUTPUT);
  //pinMode(ECHO_PIN_2, INPUT);

  // Turn off LEDs at start
  digitalWrite(LED_R_1, LOW);
  digitalWrite(LED_R_2, LOW);
  digitalWrite(LED_G_1, HIGH);
  digitalWrite(LED_G_2, HIGH);

  // Initialize the radio
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();

  Serial.println("Transmitter initialized");
}

void loop() {
  // Read distance from both ultrasonic sensors
  int IR_1_Value = digitalRead(IR_1);
  int IR_2_Value = digitalRead(IR_2);

  // Check if a vehicle is detected on one side and control LEDs accordingly
  if (IR_1_Value == 0) {
    // Vehicle detected on side 1, turn on LED 2 (stop light for side 2)
    digitalWrite(LED_R_2, HIGH);
    digitalWrite(LED_G_2, LOW);
  } else {
    // No vehicle detected on side 1, turn off LED 2
    digitalWrite(LED_R_2, LOW);
    digitalWrite(LED_G_2, HIGH);
  }

  if (IR_2_Value == 0) {
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

  // Read gyroscope values
  int16_t gx, gy, gz;
  mpu.getRotation(&gx, &gy, &gz);

  // Convert to degrees per second (assuming a full-scale range of ±250 degrees per second)
  float gyroX = gx / 131.0;
  float gyroY = gy / 131.0;
  float gyroZ = gz / 131.0;

  // Print gyroscope values
  Serial.print("GyroX: "); Serial.print(gyroX);
  Serial.print(" GyroY: "); Serial.print(gyroY);
  Serial.print(" GyroZ: "); Serial.println(gyroZ);

  // Calculate the average of gyroscope values
  float avgGyro = (abs(gyroX) + abs(gyroY) + abs(gyroZ)) / 3.0;

  // Print the average gyroscope value
  Serial.print("Average Gyro: ");
  Serial.println(avgGyro);

  // Determine the risk level and print the corresponding message
  LOW_G = 0; MIDDLE_G = 0; HIGH_G = 0; // Reset values
  if (avgGyro <= lowThreshold) {
    LOW_G = 1;
    d = 1;
  } else if (avgGyro > lowThreshold && avgGyro <= middleThreshold) {
    MIDDLE_G = 1;
    d = 2;
  } else {
    HIGH_G = 1;
    d = 3;
  }

  // Wait for 1 second before taking another reading
  delay(1000);

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

  // Send message to the receiver
  char text[32];  // Maximum payload size for nRF24L01 is 32 bytes
  snprintf(text, sizeof(text), "M:%d G:%d R:%d S:%d", a, d, b, c);
  bool success = radio.write(&text, sizeof(text));
  if (success) {
    Serial.println("Transmission successful");
  } else {
    Serial.println("Transmission failed");
  }

  // Small delay to debounce the buttons
  delay(100);
}
