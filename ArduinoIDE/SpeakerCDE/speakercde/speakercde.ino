#include <M5Stack.h>

#define NOTE_C3   131
#define NOTE_CS3  139
#define NOTE_D3   147
#define NOTE_DS3  156
#define NOTE_E3   165
#define NOTE_F3   175
#define NOTE_FS3  185
#define NOTE_G3   196
#define NOTE_GS3  208

void setup() {
  M5.begin();
  M5.Lcd.printf("M5Stack Speaker test:\r\n");
}

void loop() {
  if (M5.BtnA.wasPressed()) {
    M5.Lcd.printf("A wasPressed \r\n");
    M5.Speaker.tone(NOTE_C3, 100);
  }

  if (M5.BtnB.wasPressed()) {
    M5.Lcd.printf("B wasPressed \r\n");
    M5.Speaker.tone(NOTE_D3, 100);
  }

  if (M5.BtnC.wasPressed()) {
    M5.Lcd.printf("C wasPressed \r\n");
    M5.Speaker.tone(NOTE_E3, 100);
  }

  M5.update();
}
