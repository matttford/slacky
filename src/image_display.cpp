#include "image_display.h"

// Direct mapping of status emoji text to RGB565 image files
const EmojiImageMap emojiImages[] = {
  {"busy-interrupt-ok", "/src/assets/emojis_rgb565/busy-interrupt-ok.rgb565"},
  {"busy-interrupt-maybe", "/src/assets/emojis_rgb565/busy-interrupt-maybe.rgb565"},
  {"busy-no-interrupting", "/src/assets/emojis_rgb565/busy-no-interrupting.rgb565"},
  {"busy-stay-clear", "/src/assets/emojis_rgb565/busy-stay-clear.rgb565"},
  {"vacation", "/src/assets/emojis_rgb565/vacation.rgb565"}
};

const int numEmojiImages = sizeof(emojiImages) / sizeof(EmojiImageMap);

bool initImageSystem() {
  if (!SPIFFS.begin(true)) {
    Serial.println("Failed to mount SPIFFS");
    return false;
  }
  Serial.println("SPIFFS mounted successfully");
  return true;
}

String getImagePath(const String& statusEmoji) {
  // Remove colons and convert to lowercase for comparison
  String cleanEmoji = statusEmoji;
  cleanEmoji.replace(":", "");
  cleanEmoji.toLowerCase();
  
  for (int i = 0; i < numEmojiImages; i++) {
    if (cleanEmoji == emojiImages[i].emoji) {
      return String(emojiImages[i].imagePath);
    }
  }
  return ""; // No image found
}

bool displayEmojiImage(Adafruit_SSD1351& display, const String& statusEmoji) {
  String imagePath = getImagePath(statusEmoji);
  if (imagePath == "") {
    return false; // No image for this emoji
  }
  
  File file = SPIFFS.open(imagePath, "r");
  if (!file) {
    Serial.println("Failed to open: " + imagePath);
    return false;
  }
  
  // Verify file size (128x128x2 bytes = 32768 bytes)
  size_t expectedSize = SCREEN_WIDTH * SCREEN_HEIGHT * 2;
  if (file.size() != expectedSize) {
    Serial.println("Wrong file size for: " + imagePath);
    file.close();
    return false;
  }
  
  // Read and display image line by line to save memory
  uint16_t lineBuffer[SCREEN_WIDTH];
  
  for (int y = 0; y < SCREEN_HEIGHT; y++) {
    size_t bytesRead = file.readBytes((char*)lineBuffer, SCREEN_WIDTH * 2);
    if (bytesRead != SCREEN_WIDTH * 2) {
      Serial.println("Failed to read line " + String(y));
      file.close();
      return false;
    }
    
    // Display the line
    for (int x = 0; x < SCREEN_WIDTH; x++) {
      display.drawPixel(x, y, lineBuffer[x]);
    }
  }
  
  file.close();
  Serial.println("Displayed emoji image: " + statusEmoji);
  return true;
}