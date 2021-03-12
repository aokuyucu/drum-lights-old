// drum_config.h

#ifndef _DRUM_CONFIG_H
#define _DRUM_DRUM_CONFIG_H

enum drumID {
  Bass1,
  Bass2,
  Bass3,
  Bass4, 
  Bass5,
  Snare1,
  Snare2,
  Snare3,
  Quads1,
  Quads2
};

struct drumLight {
  drumID drumId;
  uint32_t color1;
  uint32_t color2;
  uint32_t color3;
  uint32_t color4;
  uint32_t color5;
  uint8_t brightness;
  uint32_t threshold;
  uint8_t delayValue;
  uint8_t pixelCount;
};

//drumLight = {DRUM_ID, COLOR, BRIGHTNESS, THRESHOLD, DELAY, MODE, PIXEL_COUNT};

  //Define timing for flashes, all in milliseconds
  uint8_t ShortFlash_OnMS = 100;
  uint8_t ShortFlash_OffMS = 25;
  uint8_t MediumFlash_OnMS = 300;
  uint8_t MediumFlash_OffMS = 25;
  uint8_t LongFlash_OnMS = 500;
  uint8_t LongFlash_OffMS = 50;

#endif  // _DRUM_DRUM_CONFIG_H
