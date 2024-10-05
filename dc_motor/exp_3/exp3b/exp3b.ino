#include<dht.h>
#include<LiquidCrystal.h>
#define DHT11_PIN 7
dht DHT;
const int rs=12,en=11,d4=5,d5=4,d6=3,d7=2;
LiquidCrystal lcd(rs,en,d4,d5,d6,d7);

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
lcd.begin(16, 2);
lcd.print("Temperature:");
lcd.print(DHT.temperature);
lcd.setCursor(0, 1);
lcd.print("Humidity:");
//Serial.println(DHT.temperature);
lcd.print(DHT.humidity);
}

void loop() {
  // put your main code here, to run repeatedly:
int chk = DHT.read11(DHT11_PIN);
lcd.setCursor(13, 0);
//Serial.println(DHT.temperature);
lcd.print(DHT.temperature);
lcd.setCursor(10, 1);
lcd.print(DHT.humidity);
//lcd.print(millis() / 1000);
}
