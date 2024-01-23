#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306_STM32.h>
#define OLED_RESET     -1
Adafruit_SSD1306 display(OLED_RESET);//sda b7   sck b6
void setup() {
  delay(100);
  // put your setup code here, to run once:
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
}

void loop() {
  // put your main code here, to run repeatedly:
  long start = micros();
  display.clearDisplay();
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("X    Y");
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(0, 25);
  display.println(micros() - start);
  display.display();

}
