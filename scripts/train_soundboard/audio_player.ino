static void initialise_audio() {
  theAudio = AudioClass::getInstance();
  theAudio->begin(audio_attention_cb);
  theAudio->setRenderingClockMode(AS_CLKMODE_NORMAL);
  theAudio->setPlayerMode(AS_SETPLAYER_OUTPUTDEVICE_SPHP, AS_SP_DRV_MODE_LINEOUT);  // (NOTE: Increse driver stages if needed)
  theAudio->setVolume(VOLUME_MASTER, VOLUME_PLAY0, VOLUME_PLAY1);
}

static err_t initialise_player(const AudioClass::PlayerId id) {
  Serial.printf("Initialising player%d...\n", id);

  err_t err = theAudio->initPlayer(id, AS_CODECTYPE_MP3, "/mnt/sd0/BIN", AS_SAMPLINGRATE_AUTO, AS_CHANNEL_STEREO);
  if (err != AUDIOLIB_ECODE_OK) {
    Serial.printf("ERROR: Player%d failed to initialise.\n", id);
    exit(1);
  }

  return err;
}

static File play_file(const char *filename, const AudioClass::PlayerId player_id) {
  // Open file
  File file = theSD.open(filename);
  if (!file) {
    Serial.printf("ERROR: File '%s' failed to open.\n", filename);
    exit(1);
  }
  Serial.printf("File opened: '%s'\n", filename);

  // Start reading file
  err_t err = theAudio->writeFrames(player_id, file);
  if ((err != AUDIOLIB_ECODE_OK) && (err != AUDIOLIB_ECODE_FILEEND)) {
    Serial.printf("ERROR: File Read Error =%d\n", err);
    file.close();
    exit(1);
  }

  // Start playing file
  theAudio->startPlayer(player_id);
  Serial.printf("Started playing '%s' on player%d.\n", filename, player_id);

  return file;
}

static bool continue_play(const AudioClass::PlayerId player_id, File &file) {
  err_t err = theAudio->writeFrames(player_id, file);

  if (err == AUDIOLIB_ECODE_FILEEND) {
    Serial.printf("player%d File End.\n", player_id);
  }
  if (err) {
    Serial.printf("player%d error code: %d\n", player_id, err);
    theAudio->stopPlayer(player_id);
    file.close();
    return false;
  }

  return true;
}

/**
 * @brief Audio attention callback
 *
 * When audio internal error occurs, this function will be called back.
 */
static void audio_attention_cb(const ErrorAttentionParam *atprm) {
  Serial.printf("ERROR: Audio attention callback - failed to continue writing audio frames.\n");

  if (atprm->error_code >= AS_ATTENTION_CODE_WARNING) {
    ErrEnd = true;
  }
}