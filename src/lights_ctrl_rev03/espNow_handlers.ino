typedef struct struct_message {
  char cmd[4];
  int start;
  int end;
  uint8_t R;
  uint8_t G;
  uint8_t B;
  uint8_t W;
} struct_message;

// Create a struct_message called myData
struct_message myData;

// Callback function that will be executed when data is received
void onEspNowDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&myData, incomingData, sizeof(myData));
  if (!strcmp(myData.cmd, "OUT1")) {
    out_d[0] = !out_d[0];
  }
  if (!strcmp(myData.cmd, "OUT2")) {
    out_d[1] = !out_d[1];
  }
  if (!strcmp(myData.cmd, "OUT3")) {
    out_d[2] = !out_d[2];
  }
  if (!strcmp(myData.cmd, "OUT4")) {
    out_d[3] = !out_d[3];
    toggleNpWhite(255);
  }

  if (!strcmp(myData.cmd, "CLSA"))    clearLeds();

  if (!strcmp(myData.cmd, "RGBW")) {
    RGBW[0] = myData.R;
    RGBW[1] = myData.G;
    RGBW[2] = myData.B;
    RGBW[3] = myData.W;
    npAllPixels();
  }
  if (!strcmp(myData.cmd, "rgbw")) {
    out_d[3] = true;
    digitalWrite(OUT3, out_d[3]);
    delay(100);
    RGBW[0] = myData.R;
    RGBW[1] = myData.G;
    RGBW[2] = myData.B;
    RGBW[3] = myData.W;
    for (int i = myData.start; i < myData.end; i++) {
      pixels->setPixelColor(i, pixels->Color(RGBW[0], RGBW[1], RGBW[2], RGBW[3]));
    }
    pixels->show();
  }

  if (!strcmp(myData.cmd, "out1")) {
    ch_presets[0] = myData.start;
    out_d[0] = true;
  }
  if (!strcmp(myData.cmd, "out2")) {
    ch_presets[1] = myData.start;
    out_d[1] = true;
  }
  if (!strcmp(myData.cmd, "out3")) {
    ch_presets[2] = myData.start;
    out_d[2] = true;
  }
  if (!strcmp(myData.cmd, "out4")) {
    out_d[3] = true;
    digitalWrite(OUT3, out_d[3]);
    delay(100);
    RGBW[3] = PWM_presets[myData.start];
    npAllPixels();
  }
}
