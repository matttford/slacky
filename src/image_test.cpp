#include "image_test.h"

void drawTestPattern(Adafruit_SSD1351& display) {
  Serial.println("Drawing test pattern...");
  
  // Clear screen
  display.fillScreen(0x0000);
  
  // Draw a simple test pattern that looks like an emoji
  // Draw a circle for a face
  for (int y = 32; y < 96; y++) {
    for (int x = 32; x < 96; x++) {
      int dx = x - 64;
      int dy = y - 64;
      int distance = sqrt(dx*dx + dy*dy);
      
      if (distance < 30) {
        display.drawPixel(x, y, 0xFFE0); // Yellow face
      } else if (distance < 32) {
        display.drawPixel(x, y, 0x0000); // Black border
      }
    }
  }
  
  // Draw eyes
  for (int y = 50; y < 58; y++) {
    for (int x = 52; x < 60; x++) {
      display.drawPixel(x, y, 0x0000); // Black eye
    }
    for (int x = 68; x < 76; x++) {
      display.drawPixel(x, y, 0x0000); // Black eye
    }
  }
  
  // Draw mouth
  for (int y = 70; y < 78; y++) {
    for (int x = 56; x < 72; x++) {
      if ((x >= 58 && x <= 70) && (y >= 72 && y <= 76)) {
        display.drawPixel(x, y, 0x0000); // Black mouth
      }
    }
  }
  
  // Add text
  display.setTextColor(0xFFFF);
  display.setTextSize(1);
  display.setCursor(5, 5);
  display.println("PATTERN OK");
  
  Serial.println("Test pattern complete!");
}

void testSPIFFS(Adafruit_SSD1351& display) {
  Serial.println("=== SPIFFS Test ===");
  
  // Initialize SPIFFS
  if (!SPIFFS.begin(true)) {
    Serial.println("SPIFFS mount failed!");
    display.fillScreen(0xF800); // Red screen for error
    display.setTextColor(0xFFFF);
    display.setTextSize(1);
    display.setCursor(5, 60);
    display.println("SPIFFS FAILED");
    return;
  }
  Serial.println("SPIFFS mounted OK");
  
  // List ALL files in SPIFFS recursively
  Serial.println("All files in SPIFFS:");
  File root = SPIFFS.open("/");
  if (root) {
    listDir(root, "", 0);
    root.close();
  } else {
    Serial.println("Failed to open root directory");
  }
  
  // Show SPIFFS info
  size_t totalBytes = SPIFFS.totalBytes();
  size_t usedBytes = SPIFFS.usedBytes();
  Serial.printf("SPIFFS Total: %d bytes, Used: %d bytes\n", totalBytes, usedBytes);
  
  // Display SPIFFS status on screen
  display.fillScreen(0x0000);
  display.setTextColor(0x07E0); // Green
  display.setTextSize(1);
  display.setCursor(5, 5);
  display.println("SPIFFS OK");
  display.setCursor(5, 20);
  display.printf("Total: %dKB", totalBytes/1024);
  display.setCursor(5, 35);
  display.printf("Used: %dKB", usedBytes/1024);
}

void listDir(File dir, String indent, int level) {
  if (level > 3) return; // Prevent infinite recursion
  
  while (true) {
    File entry = dir.openNextFile();
    if (!entry) break;
    
    Serial.printf("%s%s", indent.c_str(), entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
      if (level < 3) {
        listDir(entry, indent + "  ", level + 1);
      }
    } else {
      Serial.printf(" (%d bytes)\n", entry.size());
    }
    entry.close();
  }
}

void testImageDisplay(Adafruit_SSD1351& display) {
  Serial.println("=== Image Display Test ===");
  
  // First test SPIFFS
  testSPIFFS(display);
  delay(3000);
  
  // Then draw a test pattern to verify display works
  drawTestPattern(display);
  delay(3000);
  
  // Try different file paths
  String testPaths[] = {
    "/busy-interrupt-ok.rgb565",
    "/src/assets/emojis_rgb565/busy-interrupt-ok.rgb565",
    "/assets/emojis_rgb565/busy-interrupt-ok.rgb565",
    "/emojis_rgb565/busy-interrupt-ok.rgb565"
  };
  
  for (String path : testPaths) {
    Serial.println("Trying path: " + path);
    File testFile = SPIFFS.open(path, "r");
    if (testFile) {
      Serial.printf("Found file at %s (%d bytes)\n", path.c_str(), testFile.size());
      testFile.close();
      
      // If we found a file, try to display it
      if (testFile.size() == 32768) {
        displayRGB565File(display, path);
        return;
      }
    } else {
      Serial.println("File not found at: " + path);
    }
  }
  
  Serial.println("No RGB565 files found. Upload data folder to SPIFFS first.");
  display.fillScreen(0xF800); // Red
  display.setTextColor(0xFFFF);
  display.setTextSize(1);
  display.setCursor(5, 40);
  display.println("NO FILES FOUND");
  display.setCursor(5, 60);
  display.println("Upload SPIFFS");
  display.setCursor(5, 80);
  display.println("data first");
}

void displayRGB565File(Adafruit_SSD1351& display, String filePath) {
  Serial.println("Displaying file: " + filePath);
  
  File imageFile = SPIFFS.open(filePath, "r");
  if (!imageFile) {
    Serial.println("Failed to open file");
    return;
  }
  
  display.fillScreen(0x0000);
  
  // Read and display line by line
  uint16_t lineBuffer[128];
  
  for (int y = 0; y < 128; y++) {
    size_t bytesRead = imageFile.readBytes((char*)lineBuffer, 128 * 2);
    if (bytesRead != 128 * 2) {
      Serial.printf("Read error at line %d\n", y);
      break;
    }
    
    for (int x = 0; x < 128; x++) {
      display.drawPixel(x, y, lineBuffer[x]);
    }
  }
  
  imageFile.close();
  
  // Add success indicator
  display.setTextColor(0xFFFF);
  display.setTextSize(1);
  display.setCursor(5, 5);
  display.println("FILE OK");
  
  Serial.println("File display complete!");
}