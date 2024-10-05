#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <LiquidCrystal.h>

// NRF24L01 pins
#define CE_PIN 7
#define CSN_PIN 8

// Initialize the NRF24L01 radio
RF24 radio(CE_PIN, CSN_PIN);

// Address through which the two modules communicate
const byte address[6] = "00001";

// Initialize the LCD
LiquidCrystal lcd(9, 10, 5, 4, 3, 2);

void setup() {
  // Start serial communication for debugging purposes
  Serial.begin(115200);

  // Initialize the radio
  if (!radio.begin()) {
    Serial.println("NRF24L01 initialization failed");
    while (1); // Stop execution
  }

  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_LOW);
  radio.startListening();

  // Initialize the LCD
  lcd.begin(16, 2);
  lcd.print("Landslide Risk:");
  Serial.println("Receiver initialized");
}

void loop() {
  // Check if there is incoming data
  if (radio.available()) {
    char text[32] = "";
    radio.read(&text, sizeof(text));
    Serial.println(text);

    // Clear the previous content on the second line of the LCD
    lcd.setCursor(0, 1);
    lcd.print("                "); // Clear the second line

    // Display the received data
    lcd.setCursor(0, 1);
    lcd.print(text);

    // Print the received data to the serial monitor
    
  }
else {
  Serial.println("nothing");
}
  // Delay to avoid flickering and too frequent updates
  delay(1000);
 
}
