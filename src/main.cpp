#include <Arduino.h>
#include "slack_server.h"
#include "slack_status.h"

// External variables from slack_server.cpp
extern String slackToken;
extern String slackUserId;

unsigned long lastCheck = 0;
const unsigned long checkInterval = 60000; // 60 sec

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("Starting Slacky Status Monitor...");

  // Load saved configurations
  loadWiFiConfig();
  loadSlackConfig();

  // Try to connect to saved WiFi
  bool wifiConnected = connectToWiFi();
  
  if (!wifiConnected) {
    // No WiFi credentials or connection failed - start AP mode
    Serial.println("WiFi connection failed. Starting configuration mode...");
    startAPMode();
  }

  // Start web server for configuration
  startSlackServer();
  
  Serial.println("Setup complete!");
  Serial.println("Configuration available at: http://192.168.4.1 (AP mode) or device IP");
}

void loop() {
  handleSlackServer();
  
  // Handle DNS server if in AP mode
  if (WiFi.getMode() == WIFI_AP || WiFi.getMode() == WIFI_AP_STA) {
    handleDNSServer();
  }

  // Only fetch Slack status if connected to WiFi and configured
  if (WiFi.status() == WL_CONNECTED && millis() - lastCheck > checkInterval) {
    lastCheck = millis();
    
    // Fetch complete user status information (only if token and user configured)
    if (slackToken != "" && slackUserId != "") {
      SlackUserStatus userStatus = fetchSlackUserStatus();
      
      if (userStatus.isValid) {
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
    } else {
      Serial.println("Slack not configured - skipping status check");
    }
  }
}
