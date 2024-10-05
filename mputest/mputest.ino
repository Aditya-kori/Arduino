#include <Wire.h>
#include <MPU6050.h>
#include <math.h>

MPU6050 mpu;

// Define thresholds for accelerometer readings (more sensitive values)
const float lowThreshold = 1;  // Low risk threshold for acceleration in g
const float middleThreshold = 1.7;  // Middle risk threshold for acceleration in g

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
}

void loop() {
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
  if (accelMagnitude <= lowThreshold) {
    Serial.println("Landslide Risk Level: Low");
  } else if (accelMagnitude > lowThreshold && accelMagnitude <= middleThreshold) {
    Serial.println("Landslide Risk Level: Middle");
  } else {
    Serial.println("Landslide Risk Level: High");
  }

  // Reinitialize the sensor if the values are not changing significantly
  static float prevMagnitude = 0;
  if (abs(accelMagnitude - prevMagnitude) < 0.01) {
    Serial.println("Reinitializing MPU6050...");
    mpu.initialize();
  }
  prevMagnitude = accelMagnitude;

  // Wait for 1 second before taking another reading
  delay(1000);
}
