// #include "oled_display.h"

// // Create OLED object
// Adafruit_SSD1351 display = Adafruit_SSD1351(
//     SCREEN_WIDTH,
//     SCREEN_HEIGHT,
//     PIN_CS,
//     PIN_DC,
//     PIN_MOSI,
//     PIN_SCLK,
//     PIN_RST
// );


// void initOLED() {
//   Serial.println("Initializing OLED display...");
  
//   // Initialize image system
//   initImageSystem();
  
//   // Set up SPI pins
//   SPI.begin(PIN_SCLK, -1, PIN_MOSI, PIN_CS);
  
//   // Initialize OLED
//   display.begin();
  
//   // Clear screen
//   display.fillScreen(COLOR_BLACK);
  
//   // Show startup message
//   display.setTextColor(COLOR_CYAN);
//   display.setTextSize(2);
//   display.setCursor(25, 40);
//   display.println("SLACKY");
//   display.setTextSize(1);
//   display.setCursor(15, 80);
//   display.println("Starting up...");
  
//   Serial.println("OLED initialized!");
//   delay(2000);
// }

// uint16_t getStatusColor(const String& statusText) {
//   String lowerStatus = statusText;
//   lowerStatus.toLowerCase();
  
//   if (lowerStatus.indexOf("busy") >= 0) {
//     return COLOR_RED;
//   } else if (lowerStatus.indexOf("vacation") >= 0) {
//     return COLOR_BLUE;
//   } else if (lowerStatus.indexOf("meeting") >= 0) {
//     return COLOR_ORANGE;
//   } else if (lowerStatus.indexOf("lunch") >= 0 || lowerStatus.indexOf("break") >= 0) {
//     return COLOR_YELLOW;
//   } else if (lowerStatus.indexOf("home") >= 0 || lowerStatus.indexOf("wfh") >= 0) {
//     return COLOR_GREEN;
//   }
  
//   return COLOR_CYAN;
// }

// void drawStatusBackground(uint16_t color) {
//   display.fillScreen(COLOR_BLACK);
  
//   // Draw a colored border
//   display.drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, color);
//   display.drawRect(1, 1, SCREEN_WIDTH-2, SCREEN_HEIGHT-2, color);
  
//   // Draw corner accents
//   display.fillRect(0, 0, 15, 15, color);
//   display.fillRect(SCREEN_WIDTH-15, 0, 15, 15, color);
//   display.fillRect(0, SCREEN_HEIGHT-15, 15, 15, color);
//   display.fillRect(SCREEN_WIDTH-15, SCREEN_HEIGHT-15, 15, 15, color);
// }

// void displaySlackStatus(const SlackUserStatus& status) {
//   if (!status.isValid) {
//     displayConnectionStatus("No Status", COLOR_RED);
//     return;
//   }
  
//   // Try to display emoji image first
//   if (displayEmojiImage(display, status.statusEmoji)) {
//     // Image displayed successfully, add overlay text
//     display.setTextColor(COLOR_WHITE);
//     display.setTextSize(1);
//     display.setCursor(5, 8);
//     String displayName = status.displayName != "" ? status.displayName : status.realName;
//     if (displayName.length() > 18) {
//       displayName = displayName.substring(0, 15) + "...";
//     }
//     display.println(displayName);
    
//     // Online status at bottom
//     display.setCursor(5, 115);
//     display.setTextColor(status.isOnline ? COLOR_GREEN : COLOR_RED);
//     display.println(status.isOnline ? "Online" : "Offline");
//   } else {
//     // Fallback to text/color display
//     uint16_t statusColor = getStatusColor(status.statusText);
//     drawStatusBackground(statusColor);
    
//     display.setTextColor(COLOR_WHITE);
//     display.setTextSize(1);
//     display.setCursor(5, 8);
//     String displayName = status.displayName != "" ? status.displayName : status.realName;
//     if (displayName.length() > 18) {
//       displayName = displayName.substring(0, 15) + "...";
//     }
//     display.println(displayName);
    
//     // Display emoji text
//     if (status.statusEmoji != "") {
//       display.setTextColor(statusColor);
//       display.setTextSize(2);
//       display.setCursor(10, 40);
//       String emoji = status.statusEmoji;
//       emoji.replace(":", "");
//       display.println(emoji.substring(0, 12));
//     }
    
//     // Display status text
//     display.setTextColor(COLOR_WHITE);
//     display.setTextSize(1);
//     display.setCursor(5, 70);
//     String statusText = status.statusText;
//     if (statusText == "") {
//       statusText = "No status set";
//     }
//     if (statusText.length() > 18) {
//       statusText = statusText.substring(0, 15) + "...";
//     }
//     display.println(statusText);
    
//     // Online status
//     display.setCursor(5, 85);
//     display.setTextColor(status.isOnline ? COLOR_GREEN : COLOR_RED);
//     display.println(status.isOnline ? "Online" : "Offline");
//   }
// }

// void displayConnectionStatus(const String& message, uint16_t color) {
//   display.fillScreen(COLOR_BLACK);
  
//   // Draw simple border
//   display.drawRect(5, 5, SCREEN_WIDTH-10, SCREEN_HEIGHT-10, color);
  
//   display.setTextColor(color);
//   display.setTextSize(2);
  
//   // Center the message
//   int16_t x1, y1;
//   uint16_t w, h;
//   display.getTextBounds(message, 0, 0, &x1, &y1, &w, &h);
//   int x = (SCREEN_WIDTH - w) / 2;
//   int y = (SCREEN_HEIGHT - h) / 2;
  
//   display.setCursor(x, y);
//   display.println(message);
// }

// void displayConfigMode() {
//   display.fillScreen(COLOR_BLACK);
  
//   // Pulsing border effect
//   static unsigned long lastBlink = 0;
//   static bool blinkState = false;
  
//   if (millis() - lastBlink > 1000) {
//     blinkState = !blinkState;
//     lastBlink = millis();
//   }
  
//   uint16_t borderColor = blinkState ? COLOR_ORANGE : COLOR_YELLOW;
//   display.drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, borderColor);
//   display.drawRect(1, 1, SCREEN_WIDTH-2, SCREEN_HEIGHT-2, borderColor);
  
//   display.setTextColor(COLOR_YELLOW);
//   display.setTextSize(2);
//   display.setCursor(15, 30);
//   display.println("CONFIG");
//   display.setCursor(25, 50);
//   display.println("MODE");
  
//   display.setTextSize(1);
//   display.setTextColor(COLOR_WHITE);
//   display.setCursor(5, 80);
//   display.println("Connect to WiFi:");
//   display.setCursor(5, 95);
//   display.println("Slacky-XXXX");
//   display.setCursor(5, 110);
//   display.println("Pass: slacky123");
// }

// void displayWiFiConnecting() {
//   static int dots = 0;
//   static unsigned long lastUpdate = 0;
  
//   if (millis() - lastUpdate > 500) {
//     dots = (dots + 1) % 4;
//     lastUpdate = millis();
    
//     display.fillScreen(COLOR_BLACK);
//     display.setTextColor(COLOR_CYAN);
//     display.setTextSize(2);
//     display.setCursor(20, 40);
//     display.println("WiFi");
    
//     display.setCursor(5, 65);
//     display.print("Connecting");
//     for (int i = 0; i < dots; i++) {
//       display.print(".");
//     }
//   }
// }

