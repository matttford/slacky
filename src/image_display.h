#ifndef IMAGE_DISPLAY_H
#define IMAGE_DISPLAY_H

#include <Arduino.h>
#include <Adafruit_SSD1351.h>

// Include RGB565 emoji images
#include "assets/emojis_rgb565/busy_interrupt_ok.h"
#include "assets/emojis_rgb565/busy_interrupt_maybe.h"
#include "assets/emojis_rgb565/busy_no_interrupting.h"
#include "assets/emojis_rgb565/busy_stay_clear.h"
#include "assets/emojis_rgb565/vacation.h"
#include "assets/emojis_rgb565/lesssgoo.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 128

// Structure for status emoji to RGB565 array mapping
struct EmojiImageMap {
  const char* emoji;
  const uint16_t* imageData;
  uint16_t width;
  uint16_t height;
};

// Function prototypes
bool initImageSystem();
bool displayEmojiImage(Adafruit_SSD1351& display, const String& statusEmoji);
const EmojiImageMap* getEmojiImage(const String& statusEmoji);

#endif