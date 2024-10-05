#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// NRF24L01 pins
#define CE_PIN 9
#define CSN_PIN 10

// Button pins
#define BUTTON_LOW 2
#define BUTTON_MIDDLE 3
#define BUTTON_HIGH 4

// Initialize the NRF24L01 radio
RF24 radio(CE_PIN, CSN_PIN);

// Address through which the two modules communicate
const byte address[6] = "00001";

void setup() {
  // Start serial communication for debugging purposes
  Serial.begin(9600);

  // Initialize the radio
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_LOW);
  radio.stopListening();

  // Initialize the button pins
  pinMode(BUTTON_LOW, INPUT_PULLUP);
  pinMode(BUTTON_MIDDLE, INPUT_PULLUP);
  pinMode(BUTTON_HIGH, INPUT_PULLUP);
}

void loop() {
  // Check the button states
  if (digitalRead(BUTTON_LOW) == LOW) {
    sendRiskMessage("Landslide Risk: LOW");
  }
  if (digitalRead(BUTTON_MIDDLE) == LOW) {
    sendRiskMessage("Landslide Risk: MIDDLE");
  }
  if (digitalRead(BUTTON_HIGH) == LOW) {
    sendRiskMessage("Landslide Risk: HIGH");
  }

  delay(100); // Small delay to debounce the buttons
}

void sendRiskMessage(const char* message) {
  // Send the message
  radio.write(&message, sizeof(message));

  // Print the message to the serial monitor for debugging
  Serial.println(message);
}
