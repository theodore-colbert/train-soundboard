#include <ezButton.h>

#define VOL_DOWN_PIN  PIN_D02
#define STOP_PIN      PIN_D03
#define VOL_UP_PIN    PIN_D04
#define SOUND_0_PIN   PIN_D05 // Train approaching
#define SOUND_1_PIN   PIN_D06 // Train doors
#define SOUND_2_PIN   PIN_D07 // Train departing
#define SOUND_3_PIN   PIN_D08 // Train sfx (e.g. horn)
#define PREV_PIN      PIN_D09
#define NEXT_PIN      PIN_D10
#define LED_PIN       PIN_D11

#define DEBOUNCE_MS   50

ezButton    vol_down_btn(VOL_DOWN_PIN);
ezButton    stop_btn(STOP_PIN);
ezButton    vol_up_btn(VOL_UP_PIN);
ezButton    sound_0_btn(SOUND_0_PIN);
ezButton    sound_1_btn(SOUND_1_PIN);
ezButton    sound_2_btn(SOUND_2_PIN);
ezButton    sound_3_btn(SOUND_3_PIN);
ezButton    prev_btn(PREV_PIN);
ezButton    next_btn(NEXT_PIN);

ezButton*   buttons[9] = {
  &vol_down_btn, &stop_btn, &vol_up_btn,
  &sound_0_btn, &sound_1_btn, &sound_2_btn, &sound_3_btn,
  &prev_btn, &next_btn
};

static void initialise_system() {
  // Start serial and 'READY' LED
  pinMode(LED0, OUTPUT);
  Serial.begin(115200);
  while (!Serial) {
    digitalWrite(LED0, HIGH);
    delay(100);
    digitalWrite(LED0, LOW);
    delay(100);
  }
  digitalWrite(LED0, HIGH);
}

void setup() {
  initialise_system();

  for(ezButton* btn : buttons) {
    btn->setDebounceTime(DEBOUNCE_MS);
  }
}


void loop() {
  for (ezButton* btn : buttons) {
    btn->loop();
  }

  if(vol_down_btn.isPressed()) Serial.println("VOL DOWN");
}
