#include <M5Stack.h>

void setup() {
  M5.begin();

  M5.Lcd.print("Hello World!\n");
  M5.Lcd.print("M5Stack is running successfully!\n");
}

void loop() {
}
