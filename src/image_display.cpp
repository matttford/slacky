#include "image_display.h"

// Direct mapping of status emoji text to RGB565 image arrays
const EmojiImageMap emojiImages[] = {
  {"busy-interrupt-ok", busy_interrupt_ok, BUSY_INTERRUPT_OK_WIDTH, BUSY_INTERRUPT_OK_HEIGHT},
  {"busy-interrupt-maybe", busy_interrupt_maybe, BUSY_INTERRUPT_MAYBE_WIDTH, BUSY_INTERRUPT_MAYBE_HEIGHT},
  {"busy-no-interrupting", busy_no_interrupting, BUSY_NO_INTERRUPTING_WIDTH, BUSY_NO_INTERRUPTING_HEIGHT},
  {"busy-stay-clear", busy_stay_clear, BUSY_STAY_CLEAR_WIDTH, BUSY_STAY_CLEAR_HEIGHT},
  {"vacation", vacation, VACATION_WIDTH, VACATION_HEIGHT},
  {"lesssgoo", lesssgoo, LESSSGOO_WIDTH, LESSSGOO_HEIGHT}
};

const int numEmojiImages = sizeof(emojiImages) / sizeof(EmojiImageMap);

bool initImageSystem() {
  Serial.println("Image system initialized - using compiled RGB565 arrays");
  return true;
}

const EmojiImageMap* getEmojiImage(const String& statusEmoji) {
  // Remove colons and convert to lowercase for comparison
  String cleanEmoji = statusEmoji;
  cleanEmoji.replace(":", "");
  cleanEmoji.toLowerCase();
  
  for (int i = 0; i < numEmojiImages; i++) {
    if (cleanEmoji == emojiImages[i].emoji) {
      return &emojiImages[i];
    }
  }
  return nullptr; // No image found
}

bool displayEmojiImage(Adafruit_SSD1351& display, const String& statusEmoji) {
  const EmojiImageMap* emojiImage = getEmojiImage(statusEmoji);
  if (emojiImage == nullptr) {
    return false; // No image for this emoji
  }
  
  // Clear the screen first
  display.fillScreen(0x0000);
  
  // Display the RGB565 bitmap
  display.drawRGBBitmap(0, 0, emojiImage->imageData, emojiImage->width, emojiImage->height);
  
  Serial.println("Displayed emoji image: " + statusEmoji);
  return true;
}