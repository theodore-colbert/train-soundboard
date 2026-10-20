void initialise_display() {
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
}

void displayCategory(category ctgr) {
  display.clearDisplay();

  byte offsetX = charOffsetX(ctgr.name.length());
  display.setCursor(offsetX, 0);
  display.print(ctgr.name);

  offsetX = charOffsetX(ctgr.name.length());
  display.setCursor(offsetX, V_SPACING);
  display.print(ctgr.train_class);

  // TEMP
  offsetX = charOffsetX(ctgr.sound0.name.length());
  display.setCursor(offsetX, 2*V_SPACING);
  display.print(ctgr.sound0.name);

  display.display();
}

byte charOffsetX(byte numOfChars) {
  return (display.width()/2) - (numOfChars*CHAR_WIDTH)/2;
}