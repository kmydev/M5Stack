/*
 * デジタル時計(クソダサ仕様) for M5Stack
 * 2019/08/09 KOMIYA Motofumi
 * 
 * 参考にしたもの:
 * ArduinoIDE の M5Stackサンプル 'TFT_Clock_Digital'
 */

/*
A few colour codes:

code  color
0x0000  Black
0xFFFF  White
0xBDF7  Light Gray
0x7BEF  Dark Gray
0xF800  Red
0xFFE0  Yellow
0xFBE0  Orange
0x79E0  Brown
0x7E0 Green
0x7FF Cyan
0x1F  Blue
0xF81F  Pink
*/

#include <M5Stack.h>

#define TFT_GREY 0x5AEB

// for next 1 second timeout
uint32_t targetTime = 0;  

// Forward declaration needed for IDE 1.6.x
static uint8_t conv2d(const char* p); 

// Get H, M, S from compile time
uint8_t hh = conv2d(__TIME__), mm = conv2d(__TIME__ + 3) + 1, ss = conv2d(__TIME__ + 6);

byte omm = 99, oss = 00;
byte xcolon = 0, xsecs = 0;
unsigned int colour = 0;

// テキストの色
unsigned int forecolor = TFT_CYAN;
// 背景の色
unsigned int backcolor = TFT_BLACK;

void setup() {
  //Serial.begin(115200);
  M5.begin();
  // M5.Lcd.setRotation(1);
  M5.Lcd.fillScreen(backcolor);
  M5.Lcd.setTextSize(1);
  M5.Lcd.setTextColor(forecolor, backcolor);

  targetTime = millis() + 1000;
}

void loop() {
  if (targetTime < millis()) {
    // Set next update for 1 second later
    targetTime = millis() + 1000;

    // Adjust the time values by adding 1 second
    ss++;              // Advance second
    if (ss == 60) {    // Check for roll-over
      ss = 0;          // Reset seconds to zero
      omm = mm;        // Save last minute time for display update
      mm++;            // Advance minute
      if (mm > 59) {   // Check for roll-over
        mm = 0;
        hh++;          // Advance hour
        if (hh > 23) { // Check for 24hr roll-over (could roll-over on 13)
          hh = 0;      // 0 for 24 hour clock, set to 1 for 12 hour clock
        }
      }
    }


    // Update digital time
    int xpos = 0;
    int ypos = 85; // Top left corner ot clock text, about half way down
    int ysecs = ypos + 24;

    if (omm != mm) { // Redraw hours and minutes time every minute
      omm = mm;
      // Draw hours and minutes
      if (hh < 10) xpos += M5.Lcd.drawChar('0', xpos, ypos, 8); // Add hours leading zero for 24 hr clock
      xpos += M5.Lcd.drawNumber(hh, xpos, ypos, 8);             // Draw hours
      xcolon = xpos; // Save colon coord for later to flash on/off later
      xpos += M5.Lcd.drawChar(':', xpos, ypos - 8, 8);
      if (mm < 10) xpos += M5.Lcd.drawChar('0', xpos, ypos, 8); // Add minutes leading zero
      xpos += M5.Lcd.drawNumber(mm, xpos, ypos, 8);             // Draw minutes
      xsecs = xpos; // Sae seconds 'x' position for later display updates
    }
    if (oss != ss) { // Redraw seconds time every second
      oss = ss;
      xpos = xsecs;

      if (ss % 2) { // Flash the colons on/off
        M5.Lcd.setTextColor(0x39C4, backcolor);        // Set colour to grey to dim colon
        M5.Lcd.drawChar(':', xcolon, ypos - 8, 8);     // Hour:minute colon
        xpos += M5.Lcd.drawChar(':', xsecs, ysecs, 6); // Seconds colon
        M5.Lcd.setTextColor(forecolor, backcolor);    // Set colour back to yellow
      }
      else {
        M5.Lcd.drawChar(':', xcolon, ypos - 8, 8);     // Hour:minute colon
        xpos += M5.Lcd.drawChar(':', xsecs, ysecs, 6); // Seconds colon
      }

      //Draw seconds
      if (ss < 10) xpos += M5.Lcd.drawChar('0', xpos, ysecs, 6); // Add leading zero
      M5.Lcd.drawNumber(ss, xpos, ysecs, 6);                     // Draw seconds
    }
  }

  buttons_test();
  M5.update();
}


// Function to extract numbers from compile time string
static uint8_t conv2d(const char* p) {
  uint8_t v = 0;
  if ('0' <= *p && *p <= '9')
    v = *p - '0';
  return 10 * v + *++p - '0';
}

void buttons_test() {
    if(M5.BtnA.wasPressed()) {
      hh = (hh < 23) ? hh + 1 : 0;
    }
    if(M5.BtnB.wasPressed()) {
      mm = (mm < 59) ? mm + 1 : 0;
    } 
    if(M5.BtnC.wasPressed()) {
      ss = (ss < 30) ? 0 : 59;
    } 
}
