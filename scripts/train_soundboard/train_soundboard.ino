// Audio
#include <SDHCI.h>
#include <Audio.h>

// Volumne in dB
#define VOLUME_MASTER -160
#define VOLUME_PLAY0 -160
#define VOLUME_PLAY1 -160

// TEMP: Filenames
#define PLAY0_FILE_NAME "test1.mp3"
#define PLAY1_FILE_NAME "test2.mp3"

// Audio
SDClass theSD;
AudioClass *theAudio;
File file0;
File file1;
bool ErrEnd = false;

// TEMP
bool remaining0 = true;
bool remaining1 = true;

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

  Serial.printf("===== Setup Begin =====\n");

  while (!theSD.begin()) {
    Serial.printf("INPUT: Insert SD card...\n");
  }

  Serial.printf("Initialising Audio Library...\n");

  initialise_audio();

  Serial.printf("Initialising players...\n");

  initialise_player(AudioClass::Player0);
  initialise_player(AudioClass::Player1);
  file0 = play_file(PLAY0_FILE_NAME, AudioClass::Player0);
  file1 = play_file(PLAY1_FILE_NAME, AudioClass::Player1);

  Serial.printf("===== Setup End =====\n");
}

void loop() {
  // Keep sending frames until files end
  if (remaining0) remaining0 = continue_play(AudioClass::Player0, file0);
  if (remaining1) remaining1 = continue_play(AudioClass::Player1, file1);

  if (!remaining0 && !remaining1) {
    Serial.printf("Both files ended, stopping execution...\n");
    goto stop_exec;
  }

  if (ErrEnd) {
    Serial.printf("ERROR: Unexpected error, stopping execution...\n");
    goto stop_exec;
  }

  // Suspend execution of calling thread (sleep) before writing next frames.
  // (Adjust depending on amount of processing required in thread)
  usleep(40000);

  // Continue play...
  return;

stop_exec:
  Serial.printf("===== Execution Stopped ====\n");
  theAudio->setReadyMode();
  theAudio->end();
  exit(1);
}
