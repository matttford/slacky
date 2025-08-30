#include "slack_server.h"
#include "slack_status.h"

WebServer server(80);
DNSServer dnsServer;

String slackUserId = "";
String slackToken = "";
String wifiSSID = "";
String wifiPassword = "";

// Handle root page
void handleRoot() {
  String html = "<html><head><style>"
                "body { font-family: Arial, sans-serif; margin: 40px; }"
                "h1, h2 { color: #333; }"
                "form { background: #f5f5f5; padding: 20px; border-radius: 5px; margin: 10px 0; }"
                "input[type=text], input[type=password] { width: 300px; padding: 8px; margin: 5px 0; }"
                "input[type=submit] { background: #4CAF50; color: white; padding: 10px 20px; border: none; cursor: pointer; }"
                "input[type=submit]:hover { background: #45a049; }"
                ".status { background: #e7f3ff; padding: 10px; border-radius: 5px; }"
                "</style></head><body>"
                "<h1>Slacky Device Configuration</h1>"
                
                "<h2>WiFi Settings</h2>"
                "<form action='/save-wifi' method='POST'>"
                "WiFi SSID: <input type='text' name='ssid' value='" + wifiSSID + "' required><br>"
                "WiFi Password: <input type='password' name='password' value='" + wifiPassword + "'><br><br>"
                "<input type='submit' value='Save WiFi Config'>"
                "</form>"
                "<div class='status'>Current WiFi: " + (wifiSSID != "" ? wifiSSID : "Not configured") + "</div>"
                
                "<h2>Slack Settings</h2>"
                "<form action='/save-slack' method='POST'>"
                "Slack Bot Token: <input type='password' name='token' value='" + (slackToken != "" ? "••••••••" : "") + "' placeholder='xoxb-your-bot-token'><br>"
                "Slack User ID: <input type='text' name='userid' value='" + slackUserId + "' placeholder='U1234567890'><br><br>"
                "<input type='submit' value='Save Slack Config'>"
                "</form>"
                "<div class='status'>User ID: " + (slackUserId != "" ? slackUserId : "Not configured") + "<br>"
                "Token: " + (slackToken != "" ? "Configured" : "Not configured") + "</div>"
                
                "<hr>"
                "<h2>Current Slack Status</h2>"
                "<div id='current-status'>";
  
  // Add current status if configured
  if (slackToken != "" && slackUserId != "") {
    SlackUserStatus status = fetchSlackUserStatus();
    if (status.isValid) {
      html += "<div style='background: #e8f5e8; padding: 15px; border-radius: 5px; border-left: 5px solid #4CAF50;'>";
      html += "<h3 style='margin: 0; color: #2e7d32;'>✅ Status Retrieved</h3>";
      html += "<table style='width: 100%; margin-top: 10px;'>";
      html += "<tr><td><strong>Display Name:</strong></td><td>" + status.displayName + "</td></tr>";
      html += "<tr><td><strong>Real Name:</strong></td><td>" + status.realName + "</td></tr>";
      html += "<tr><td><strong>Status Text:</strong></td><td>" + (status.statusText != "" ? status.statusText : "No status") + "</td></tr>";
      html += "<tr><td><strong>Status Emoji:</strong></td><td>" + (status.statusEmoji != "" ? status.statusEmoji : "No emoji") + "</td></tr>";
      html += "<tr><td><strong>Is Online:</strong></td><td>" + String(status.isOnline ? "🟢 Yes" : "🔴 No") + "</td></tr>";
      html += "</table></div>";
    } else {
      html += "<div style='background: #ffebee; padding: 15px; border-radius: 5px; border-left: 5px solid #f44336;'>";
      html += "<h3 style='margin: 0; color: #c62828;'>❌ Failed to retrieve status</h3>";
      html += "<p>Check your Slack token and user ID configuration.</p>";
      html += "</div>";
    }
  } else {
    html += "<div style='background: #fff3e0; padding: 15px; border-radius: 5px; border-left: 5px solid #ff9800;'>";
    html += "<h3 style='margin: 0; color: #ef6c00;'>⚠️ Configuration Required</h3>";
    html += "<p>Configure both Slack Bot Token and User ID above to see status.</p>";
    html += "</div>";
  }
  
  html += "</div>"
       "<script>"
       "setTimeout(function(){ location.reload(); }, 30000);" // Auto-refresh every 30 seconds
       "</script>";
                "</body></html>";
  server.send(200, "text/html", html);
}

// Handle WiFi configuration save
void handleSaveWiFi() {
  if (server.hasArg("ssid")) {
    wifiSSID = server.arg("ssid");
    wifiPassword = server.arg("password");
    saveWiFiConfig();
    
    server.send(200, "text/html", 
      "<html><body style='font-family: Arial, sans-serif; margin: 40px;'>"
      "<h1>WiFi Configuration Saved!</h1>"
      "<p>SSID: " + wifiSSID + "</p>"
      "<p>Password: " + (wifiPassword != "" ? "••••••••" : "None") + "</p>"
      "<p><strong>Note:</strong> Device will restart to apply new WiFi settings.</p>"
      "<a href='/'>Back to Config</a>"
      "</body></html>");
    
    Serial.println("Saved WiFi - SSID: " + wifiSSID);
    
    // Restart after a delay to apply new WiFi settings
    delay(2000);
    ESP.restart();
  } else {
    server.send(400, "text/plain", "Missing SSID parameter");
  }
}

// Handle Slack configuration save
void handleSaveSlack() {
  bool updated = false;
  
  if (server.hasArg("token") && server.arg("token") != "••••••••") {
    slackToken = server.arg("token");
    updated = true;
  }
  
  if (server.hasArg("userid")) {
    slackUserId = server.arg("userid");
    updated = true;
  }
  
  if (updated) {
    saveSlackConfig();
    server.send(200, "text/html", 
      "<html><body style='font-family: Arial, sans-serif; margin: 40px;'>"
      "<h1>Slack Configuration Saved!</h1>"
      "<p>User ID: " + slackUserId + "</p>"
      "<p>Token: " + (slackToken != "" ? "Configured" : "Not configured") + "</p>"
      "<a href='/'>Back to Config</a>"
      "</body></html>");
    
    Serial.println("Saved Slack config - User ID: " + slackUserId);
  } else {
    server.send(400, "text/plain", "No valid parameters provided");
  }
}

// Handle status query
void handleStatusQuery() {
  String queryUserId;
  
  if (server.hasArg("user") && server.arg("user") != "") {
    queryUserId = server.arg("user");
  } else if (slackUserId != "") {
    queryUserId = slackUserId;
  } else {
    server.send(400, "text/html", 
      "<html><body style='font-family: Arial, sans-serif; margin: 40px;'>"
      "<h1>Error</h1>"
      "<p>No user ID provided and no default user configured</p>"
      "<a href='/'>Back to Config</a>"
      "</body></html>");
    return;
  }
  SlackUserStatus status = fetchSlackUserStatus(queryUserId);
  
  String html = "<html><body><h1>Slack User Status</h1>";
  
  if (status.isValid) {
    html += "<table border='1' style='border-collapse: collapse;'>";
    html += "<tr><td><strong>User ID:</strong></td><td>" + status.userId + "</td></tr>";
    html += "<tr><td><strong>Display Name:</strong></td><td>" + status.displayName + "</td></tr>";
    html += "<tr><td><strong>Real Name:</strong></td><td>" + status.realName + "</td></tr>";
    html += "<tr><td><strong>Status Text:</strong></td><td>" + status.statusText + "</td></tr>";
    html += "<tr><td><strong>Status Emoji:</strong></td><td>" + status.statusEmoji + "</td></tr>";
    html += "<tr><td><strong>Is Online:</strong></td><td>" + String(status.isOnline ? "Yes" : "No") + "</td></tr>";
    if (status.statusExpiration != "") {
      html += "<tr><td><strong>Status Expiration:</strong></td><td>" + status.statusExpiration + "</td></tr>";
    }
    html += "</table>";
  } else {
    html += "<p style='color: red;'>Failed to retrieve user status. Check the user ID and try again.</p>";
  }
  
  html += "<br><a href='/'>Back to Config</a></body></html>";
  server.send(200, "text/html", html);
}

void startSlackServer() {
  server.on("/", handleRoot);
  server.on("/save-wifi", HTTP_POST, handleSaveWiFi);
  server.on("/save-slack", HTTP_POST, handleSaveSlack);
  server.on("/status", HTTP_GET, handleStatusQuery);
  server.begin();
  Serial.println("Configuration web server started!");
}

void handleSlackServer() {
  server.handleClient();  // Handle incoming requests
}

void saveWiFiConfig() {
  Preferences prefs;
  prefs.begin("wifi", false);
  prefs.putString("ssid", wifiSSID);
  prefs.putString("password", wifiPassword);
  prefs.end();
  Serial.println("WiFi config saved to NVS");
}

void loadWiFiConfig() {
  Preferences prefs;
  prefs.begin("wifi", true);
  wifiSSID = prefs.getString("ssid", "");
  wifiPassword = prefs.getString("password", "");
  prefs.end();
  Serial.println("WiFi config loaded from NVS");
}

void saveSlackConfig() {
  Preferences prefs;
  prefs.begin("slack", false);
  prefs.putString("userid", slackUserId);
  prefs.putString("token", slackToken);
  prefs.end();
  Serial.println("Slack config saved to NVS");
}

void loadSlackConfig() {
  Preferences prefs;
  prefs.begin("slack", true);
  slackUserId = prefs.getString("userid", "");
  slackToken = prefs.getString("token", "");
  prefs.end();
  Serial.println("Slack config loaded from NVS");
}

bool connectToWiFi() {
  if (wifiSSID == "") {
    Serial.println("No WiFi credentials configured");
    return false;
  }
  
  Serial.println("Connecting to WiFi: " + wifiSSID);
  WiFi.begin(wifiSSID.c_str(), wifiPassword.c_str());
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi connected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    return true;
  } else {
    Serial.println("\nWiFi connection failed");
    return false;
  }
}

bool startAPMode() {
  Serial.println("Starting Access Point mode...");
  
  WiFi.mode(WIFI_AP);
  
  String apName = "Slacky-" + String(ESP.getEfuseMac() & 0xFFFF, HEX);
  apName.toUpperCase();
  
  bool success = WiFi.softAP(apName.c_str(), "slacky123");
  
  if (success) {
    Serial.println("AP started successfully");
    Serial.println("AP Name: " + apName);
    Serial.println("AP Password: slacky123");
    Serial.print("AP IP address: ");
    Serial.println(WiFi.softAPIP());
    
    // Start captive portal DNS server
    dnsServer.start(53, "*", WiFi.softAPIP());
    
    return true;
  } else {
    Serial.println("Failed to start AP mode");
    return false;
  }
}

void handleDNSServer() {
  dnsServer.processNextRequest();
}
