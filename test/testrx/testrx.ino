#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Define pins for NRF24L01
#define CE_PIN 9
#define CSN_PIN 10

// Initialize the NRF24L01 radio
RF24 radio(CE_PIN, CSN_PIN);

// Address through which the two modules communicate
const byte address[6] = "00001";

void setup() {
  // Start serial communication at 115200 baud rate
  Serial.begin(115200);

  // Initialize the radio
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

  Serial.println("Receiver initialized");
}

void loop() {
  if (radio.available()) {
    char text[32] = "";
    radio.read(&text, sizeof(text));
    Serial.println(text);
  }

  // Small delay to avoid flooding the serial monitor
  delay(100);
}
