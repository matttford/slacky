#ifndef IMAGE_DISPLAY_H
#define IMAGE_DISPLAY_H

#include <Arduino.h>
#include <Adafruit_SSD1351.h>
#include <SPIFFS.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 128

// Simple structure for status emoji to image mapping
struct EmojiImageMap {
  const char* emoji;
  const char* imagePath;
};

// Function prototypes
bool initImageSystem();
bool displayEmojiImage(Adafruit_SSD1351& display, const String& statusEmoji);
String getImagePath(const String& statusEmoji);

#endif