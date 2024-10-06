#include <AFMotor.h>    // Adafruit Motor Shield library for controlling motors
#include <SoftwareSerial.h>  // For Bluetooth communication
#include <Servo.h>      // For controlling the servo motor

// Motor setup: Define motors connected to M1, M2, M3, M4
AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);
AF_DCMotor motor4(4);

// Bluetooth module pins (TX, RX)
SoftwareSerial bluetooth(10, 11);  // TX, RX

// Ultrasonic sensor pins
const int trigPin = 7;
const int echoPin = 6;

// IR sensor pin (back obstacle detection)
const int irSensorPin = 8;

// Servo motor for rotating ultrasonic sensor
Servo ultrasonicServo;

// Variables
long duration;
int distance;
int frontDistance;

// Function declarations
void moveForward();
void moveBackward();
void turnLeft(int degrees);
void turnRight(int degrees);
void stopMotors();
int getUltrasonicDistance();
bool isRearObstacle();
bool checkForObstacleLeft();
bool checkForObstacleRight();

void setup() {
  Serial.begin(9600);       // Start serial communication for debugging
  bluetooth.begin(9600);    // Start Bluetooth communication at 9600 baud rate
  Serial.println("Waiting for Bluetooth commands...");

  // Initialize motors
  motor1.setSpeed(150);
  motor2.setSpeed(150);
  motor3.setSpeed(150);
  motor4.setSpeed(150);

  // Ultrasonic sensor and servo setup
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  ultrasonicServo.attach(9);  // Attach servo to pin 9
  ultrasonicServo.write(90);   // Start with the sensor facing forward

  // IR sensor setup
  pinMode(irSensorPin, INPUT);
}

void loop() {
  if (bluetooth.available()) {
    String command = bluetooth.readStringUntil('\n');  // Read command from Bluetooth
    command.trim();  // Remove any extra spaces or newline characters
    Serial.println("Received command: " + command);

    // Check for obstacles in front using ultrasonic sensor
    frontDistance = getUltrasonicDistance();
    bool rearObstacle = isRearObstacle();

    if (frontDistance < 20) {
      Serial.println("Obstacle detected in front, stopping.");
      stopMotors();  // Stop if obstacle detected
      return;
    }

    if (rearObstacle) {
      Serial.println("Obstacle detected in the rear, stopping.");
      stopMotors();
      return;
    }

    // Process the command
    if (command == "forward") {
      moveForward();
    } else if (command == "backward") {
      moveBackward();
    } else if (command == "left") {
      turnLeft(90);  // Default turn left 90 degrees
    } else if (command == "right") {
      turnRight(90); // Default turn right 90 degrees
    } else if (command == "stop") {
      stopMotors();
    } else if (command.startsWith("left ")) {
      int angle = command.substring(5).toInt();
      turnLeft(angle);  // Turn left by the specified angle
    } else if (command.startsWith("right ")) {
      int angle = command.substring(6).toInt();
      turnRight(angle); // Turn right by the specified angle
    }
  }
}

// Move the car forward
void moveForward() {
  Serial.println("Moving forward...");
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
}

// Move the car backward
void moveBackward() {
  Serial.println("Moving backward...");
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
}

// Turn left by a specific angle with obstacle checking
void turnLeft(int degrees) {
  Serial.println("Checking for obstacles on the left...");

  if (checkForObstacleLeft()) {
    Serial.println("Obstacle detected on the left. Turn aborted.");
    stopMotors();
  } else {
    Serial.println("Turning left by " + String(degrees) + " degrees...");
    motor1.run(BACKWARD);
    motor2.run(BACKWARD);
    motor3.run(FORWARD);
    motor4.run(FORWARD);
    delay(degrees * 10);  // Adjust timing to control turn accuracy
    stopMotors();
  }
}

// Turn right by a specific angle with obstacle checking
void turnRight(int degrees) {
  Serial.println("Checking for obstacles on the right...");

  if (checkForObstacleRight()) {
    Serial.println("Obstacle detected on the right. Turn aborted.");
    stopMotors();
  } else {
    Serial.println("Turning right by " + String(degrees) + " degrees...");
    motor1.run(FORWARD);
    motor2.run(FORWARD);
    motor3.run(BACKWARD);
    motor4.run(BACKWARD);
    delay(degrees * 10);  // Adjust timing to control turn accuracy
    stopMotors();
  }
}

// Stop all motors
void stopMotors() {
  Serial.println("Stopping motors...");
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
}

// Get the distance to the nearest obstacle using the ultrasonic sensor
int getUltrasonicDistance() {
  // Send trigger pulse
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read echo pulse
  duration = pulseIn(echoPin, HIGH);
  
  // Calculate distance (in cm)
  distance = duration * 0.034 / 2;

  return distance;
}

// Check if there's an obstacle in the rear using the IR sensor
bool isRearObstacle() {
  return digitalRead(irSensorPin) == LOW;  // LOW means an obstacle is detected
}

// Check for obstacles on the left
bool checkForObstacleLeft() {
  ultrasonicServo.write(0);  // Turn the servo to the left
  delay(500);  // Wait for the servo to move
  int distance = getUltrasonicDistance();
  ultrasonicServo.write(90);  // Return the sensor to the front position
  delay(500);
  return distance < 20;  // Return true if obstacle is detected within 20 cm
}

// Check for obstacles on the right
bool checkForObstacleRight() {
  ultrasonicServo.write(180);  // Turn the servo to the right
  delay(500);  // Wait for the servo to move
  int distance = getUltrasonicDistance();
  ultrasonicServo.write(90);  // Return the sensor to the front position
  delay(500);
  return distance < 20;  // Return true if obstacle is detected within 20 cm
}
