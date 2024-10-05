#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <LiquidCrystal.h>

// NRF24L01 pins
#define CE_PIN 9
#define CSN_PIN 10

// Initialize the NRF24L01 radio
RF24 radio(CE_PIN, CSN_PIN);

// Address through which the two modules communicate
const byte address[6] = "00001";

// Initialize the LCD
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

void setup() {
  // Start serial communication for debugging purposes
  Serial.begin(115200);

  // Initialize the radio
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

  // Initialize the LCD
  lcd.begin(16, 2);
  
  Serial.print("Landslide Risk:");
  Serial.println("Receiver initialized");
}

void loop() {
  

    // Clear the previous content
    lcd.setCursor(0,0);
  lcd.print("Landslide Risk:");
    lcd.setCursor(0, 1);
    lcd.print("text"); // Clear the second line

    // Display the received data
    lcd.setCursor(0, 1);
    lcd.print("text");

    // Print the received data to the serial monitor
    Serial.println("text");
  
  delay(500);
}
