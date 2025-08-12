#include <Arduino.h>
#include "slack_server.h"
#include "slack_status.h"

unsigned long lastCheck = 0;
const unsigned long checkInterval = 60000; // 60 sec

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("Starting Slack Status Monitor...");


  WiFi.begin("", "");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Start web server for User ID input
  startSlackServer();

  // Load any saved Slack config
  loadSlackConfig();
}

void loop() {
  handleSlackServer();

  // Periodically fetch status
  if (WiFi.status() == WL_CONNECTED && millis() - lastCheck > checkInterval) {
    lastCheck = millis();
    
    // Fetch complete user status information
    SlackUserStatus userStatus = fetchSlackUserStatus();
    
    if (userStatus.isValid) {
      // You can also fetch status for a specific user ID like this:
      // SlackUserStatus specificUserStatus = fetchSlackUserStatus("U1234567890");
      
      // Map status emoji to colors for display
      String displayColor = "Red"; // Default
      if (userStatus.statusEmoji == ":house:") {
        displayColor = "Green"; // Working from home
      } else if (userStatus.statusEmoji == ":palm_tree:") {
        displayColor = "Blue"; // On vacation
      } else if (userStatus.statusEmoji == ":spiral_calendar_pad:") {
        displayColor = "Orange"; // In a meeting
      } else if (userStatus.statusEmoji == ":zzz:") {
        displayColor = "Purple"; // Away/sleeping
      }
      
      Serial.println("Display Color: " + displayColor);
      // TODO: Set OLED/LED color based on displayColor
    } else {
      Serial.println("Failed to fetch valid user status");
    }
  }
}
