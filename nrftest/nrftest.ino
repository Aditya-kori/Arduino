#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10); // CE, CSN

const byte address[6] = "00001";

void setup() {
  Serial.begin(115200);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}

void loop() {
  const char text[] = "Hello World";
  bool success=radio.write(&text, sizeof(text));
  delay(1000);

  if (success) {
    Serial.println("Message sent successfully");
  } else {
    Serial.println("Message sending failed");
  }

  delay(1000); // Send message every second
}
