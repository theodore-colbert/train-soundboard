// Audio
#include <SDHCI.h>
#include <Audio.h>
// Display
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// BEGIN Audio
// Volumne in dB
#define VOLUME_MASTER -160
#define VOLUME_PLAY0 -160
#define VOLUME_PLAY1 -160
// TEMP: Filenames
#define PLAY0_FILE_NAME "test1.mp3"
#define PLAY1_FILE_NAME "test2.mp3"

SDClass theSD;
AudioClass *theAudio;
File file0;
File file1;
bool ErrEnd = false;
// TEMP
bool remaining0 = true;
bool remaining1 = true;
// END

// BEGIN Display - REQUIRES 3.3V MODE FOR SPRESENSE!
#define SCREEN_WIDTH    128
#define SCREEN_HEIGHT   64
#define OLED_RESET      -1
#define SCREEN_ADDRESS  0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define CHAR_WIDTH      6   // Width of chars in pixels + 1 pixel space
#define CHAR_HEIGHT     7   // Height of chars in pixels
#define V_SPACING       16
// END

struct sound {
  String  name;
  String  filename;
};

struct category {
  String  name;
  String  train_class;
  sound   sound0;
  sound   sound1;
  sound   sound2;
  sound   sound3;
};

category  playlist[2] = {
  {
    "Environment",
    "",
    {
      "Busy Indoor Station",
      "environment/bbc_euston_station_general.mp3"
    },
    {
      "Calm
      
      
      
      
      
      
      
      
      
      
      
      
      
      
       Indoor Station",
      "environment/bbc_victoria_station_calm.mp3"
    },
    {
      "Outdoor Station",
      "environment/bbc_country_diesel_terminus.mp3"
    },
    {
      "Underground Station",
      "environment/bbc_london_underground_night.mp3"
    },
  },
  {
    "Manchester Metrolink",
    "Bombardier M5000",
    {
      "Arriving",
      "manchester_metrolink/manchester_tram_arriving_failsworth.mp3"
    },
    {
      "Doors",
      "manchester_metrolink/manchester_tram_doors_eccles.mp3"
    },
    {
      "Departing",
      "manchester_metrolink/manchester_tram_departing_failsworth.mp3"
    },
    {
      "Horn",
      "manchester_metrolink/manchester_tram_roblox_horn.mp3"
    },
  }
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

  Serial.printf("===== Setup Begin =====\n");

  while (!theSD.begin()) {
    Serial.printf("INPUT: Insert SD card...\n");
  }

  Serial.printf("Initialising Audio Library...\n");

  initialise_audio();

  Serial.printf("Initialising players...\n");

  initialise_player(AudioClass::Player0);
  initialise_player(AudioClass::Player1);
  // file0 = play_file(PLAY0_FILE_NAME, AudioClass::Player0);
  // file1 = play_file(PLAY1_FILE_NAME, AudioClass::Player1);

  Serial.printf("Initialising display...\n");

  initialise_display();
  displayCategory(playlist[0]);
  display.display();

  Serial.printf("===== Setup End =====\n");
}

void loop() {



  // // Keep sending frames until files end
  // if (remaining0) remaining0 = continue_play(AudioClass::Player0, file0);
  // if (remaining1) remaining1 = continue_play(AudioClass::Player1, file1);

  // if (!remaining0 && !remaining1) {
  //   Serial.printf("Both files ended, stopping execution...\n");
  //   goto stop_exec;
  // }

  // if (ErrEnd) {
  //   Serial.printf("ERROR: Unexpected error, stopping execution...\n");
  //   goto stop_exec;
  // }

  // // Suspend execution of calling thread (sleep) before writing next frames.
  // // (Adjust depending on amount of processing required in thread)
  // usleep(40000);

  // Continue play...
  return;

stop_exec:
  Serial.printf("===== Execution Stopped ====\n");
  theAudio->setReadyMode();
  theAudio->end();
  exit(1);
}
