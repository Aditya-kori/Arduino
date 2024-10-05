#include <Servo.h>
#include <SoftwareSerial.h>

// Pin Definitions
#define trigPin 9
#define echoPin 10
#define servoPin 8
#define irSensorPin A0
#define motor1Pin1 2
#define motor1Pin2 3
#define motor2Pin1 4
#define motor2Pin2 5

// Constants
#define maxDistance 20   // cm (distance threshold for obstacle avoidance)
#define servoLeftAngle 180
#define servoRightAngle 0
#define turnDuration 10  // adjust based on motor power and wheelbase for precise turns

Servo myServo;  // Servo object for ultrasonic sensor
SoftwareSerial bluetooth(6, 7);  // RX, TX for Bluetooth communication

String command = "";  // String to store the received Bluetooth command

// Function Prototypes
void forward();
void backward();
void stop();
void turnLeft(int angle);
void turnRight(int angle);
long readUltrasonicDistance();

void setup() {
  // Motor Pins
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
  
  // Ultrasonic Sensor Pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  // IR Sensor Pin
  pinMode(irSensorPin, INPUT);
  
  // Attach Servo
  myServo.attach(servoPin);
  myServo.write(90);  // Center the servo

  // Initialize Bluetooth communication
  bluetooth.begin(9600); 
  Serial.begin(9600);  // For debugging
  
  Serial.println("Ready for voice commands...");
}

void loop() {
  // Check if data is available from Bluetooth
  if (bluetooth.available()) {
    char received = bluetooth.read();  // Read incoming character
    if (received != '\n') {
      command += received;  // Build command string
    } else {
      command.trim();  // Remove any unwanted characters like spaces or newlines
      Serial.println("Command Received: " + command);  // Debug output
      
      // Act on the command
      if (command == " move forward") {
        forward();
      } else if (command == " move backward") {
        backward();
      } else if (command == "stop") {
        stop();
      } else if (command == "turn left 90") {
        turnLeft(90);
      } else if (command == "turn right 90") {
        turnRight(90);
      } else if (command == "turn left 45") {
        turnLeft(45);
      } else if (command == "turn right 45") {
        turnRight(45);
      } else {
        Serial.println("Invalid command");
      }
      
      command = "";  // Clear the command after executing
    }
  }

  // Automatic obstacle detection
  long distance = readUltrasonicDistance();
  if (distance <= maxDistance) {
    stop();
    Serial.println("Obstacle detected! Avoiding...");
    myServo.write(servoLeftAngle);
    delay(500);
    long leftDistance = readUltrasonicDistance();
    
    myServo.write(servoRightAngle);
    delay(500);
    long rightDistance = readUltrasonicDistance();
    
    myServo.write(90); // Center again
    
    // Choose direction to avoid obstacle
    if (leftDistance > rightDistance) {
      turnLeft(90); // Turn left 90 degrees
    } else {
      turnRight(90); // Turn right 90 degrees
    }
    forward();  // Continue moving forward after turning
  } else if (analogRead(irSensorPin) == LOW) {  // Obstacle detected by IR when moving backward
    stop();
    forward();
  }
}

// Function to move forward
void forward() {
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);
  Serial.println("Moving forward...");
}

// Function to move backward
void backward() {
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
  Serial.println("Moving backward...");
}

// Function to stop the car
void stop() {
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);
  Serial.println("Stopped.");
}

// Function to turn left by a specified angle
void turnLeft(int angle) {
  int turnTime = map(angle, 0, 360, 0, turnDuration * 10);  // Adjust timing for the turn
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);
  delay(turnTime);
  stop();
  Serial.println("Turned left by " + String(angle) + " degrees.");
}

// Function to turn right by a specified angle
void turnRight(int angle) {
  int turnTime = map(angle, 0, 360, 0, turnDuration * 10);  // Adjust timing for the turn
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
  delay(turnTime);
  stop();
  Serial.println("Turned right by " + String(angle) + " degrees.");
}

// Function to read the distance from the ultrasonic sensor
long readUltrasonicDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  long duration = pulseIn(echoPin, HIGH);
  long distance = (duration / 2) / 29.1;  // Convert to cm
  
  return distance;
}
