#include <M5Stack.h>

int LEDPinNo = 22;

void setup() {
  // Initialize the M5Stack object
  M5.begin();

  M5.Lcd.printf("M5Stack LED Blink test:\r\n");

  pinMode(LEDPinNo, OUTPUT); // 22pin OUTPUT mode
}

void loop() {
  if (M5.BtnA.wasPressed())
  {
    M5.Lcd.printf("wasPressed A\r\n");
    M5.Lcd.printf("LED ON\r\n");
    digitalWrite(LEDPinNo, HIGH);
  }

  if (M5.BtnB.wasPressed())
  {
    M5.Lcd.printf("wasPressed B\r\n");
    M5.Lcd.printf("LED OFF\r\n");
    digitalWrite(LEDPinNo, LOW);
  }

  M5.update();
}
