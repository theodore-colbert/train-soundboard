#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

/* Display setup - REQUIRES 3.3V MODE FOR SPRESENSE! */
#define SCREEN_WIDTH    128
#define SCREEN_HEIGHT   64
#define OLED_RESET      -1
#define SCREEN_ADDRESS  0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define CHAR_WIDTH      6   // Width of chars in pixels + 1 pixel space
#define CHAR_HEIGHT     7   // Height of chars in pixels
#define V_SPACING       16

/* Serial Communication */
#define DELIMETER '\n'
char received[20];
byte length = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("Serial connected.");

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  Serial.println("Starting display...");

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  printCentred("Environment", 11);
  display.display();
}

void loop() {
  while (Serial.available() > 0) {
    char rc = Serial.read();
    if (rc != DELIMETER) received[length++] = rc;
    else {
      // Full message received
      printCentred(received, length);
      memset(received, 0, sizeof(received));
      length = 0;
    }
  }
}

void printCentred(char* received, byte length) {
  display.clearDisplay();

  byte offsetX = charOffsetX(length);
  display.setCursor(offsetX, 0);
  display.print(received);
  display.setCursor(offsetX, V_SPACING);
  display.print(received);
  display.setCursor(offsetX, 2*V_SPACING);
  display.print(received);

  display.display();
}

byte charOffsetX(byte numOfChars) {
  return (display.width()/2) - (numOfChars*CHAR_WIDTH)/2;
}

// byte charOffsetY(byte centre) {
//   return (centre - (CHAR_HEIGHT-1)/2);
// }
