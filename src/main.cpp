#include <Arduino.h>
#include "slack_server.h"
#include "slack_status.h"
#include "oled_display.h"

// External variables from slack_server.cpp
extern String slackToken;
extern String slackUserId;

unsigned long lastCheck = 0;
const unsigned long checkInterval = 60000; // 60 sec

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("Starting Slacky Status Monitor...");

  // Initialize OLED display first
  initOLED();

  // Load saved configurations
  loadWiFiConfig();
  loadSlackConfig();

  // Try to connect to saved WiFi
  displayWiFiConnecting();
  bool wifiConnected = connectToWiFi();
  
  if (!wifiConnected) {
    // No WiFi credentials or connection failed - start AP mode
    Serial.println("WiFi connection failed. Starting configuration mode...");
    startAPMode();
    String apIP = WiFi.softAPIP().toString();
    displayConfigMode(apIP);
  } else {
    displayConnectionStatus("WiFi OK", COLOR_GREEN);
    delay(1000);
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
    
    // Keep showing config mode on OLED
    static unsigned long lastConfigUpdate = 0;
    if (millis() - lastConfigUpdate > 2000) {
      String apIP = WiFi.softAPIP().toString();
      displayConfigMode(apIP);
      lastConfigUpdate = millis();
    }
  }

  // Only fetch Slack status if connected to WiFi and configured
  if (WiFi.status() == WL_CONNECTED && millis() - lastCheck > checkInterval) {
    lastCheck = millis();
    
    // Fetch complete user status information (only if token and user configured)
    if (slackToken != "" && slackUserId != "") {
      Serial.println("Fetching Slack status...");
      SlackUserStatus userStatus = fetchSlackUserStatus();
      
      // Display status on OLED with IP address
      String currentIP = WiFi.localIP().toString();
      displaySlackStatus(userStatus, currentIP);
      
      if (userStatus.isValid) {
        Serial.println("Status displayed on OLED");
      } else {
        Serial.println("Failed to fetch valid user status - showing error on OLED");
      }
    } else {
      Serial.println("Slack not configured - showing config message on OLED");
      displayConnectionStatus("Configure\nSlack", COLOR_ORANGE);
    }
  }
  
  // Small delay to prevent overwhelming the loop
  delay(100);
}
