#include "slack_status.h"

// Helper function to extract JSON string value
String extractJsonString(const String& json, const String& key) {
  String searchKey = "\"" + key + "\":\"";
  int idx = json.indexOf(searchKey);
  if (idx == -1) return "";
  
  idx += searchKey.length();
  int endIdx = idx;
  
  // Handle escaped quotes in JSON
  while (endIdx < json.length()) {
    if (json.charAt(endIdx) == '\\') {
      endIdx += 2; // Skip escaped character
      continue;
    }
    if (json.charAt(endIdx) == '"') {
      break;
    }
    endIdx++;
  }
  
  return json.substring(idx, endIdx);
}

// Helper function to extract JSON boolean value
bool extractJsonBool(const String& json, const String& key) {
  String searchKey = "\"" + key + "\":";
  int idx = json.indexOf(searchKey);
  if (idx == -1) return false;
  
  idx += searchKey.length();
  return json.substring(idx, idx + 4) == "true";
}

// Make HTTP request to Slack API
String makeSlackRequest(const String& endpoint, const String& userId = "") {
  if (slackToken == "") {
    Serial.println("Slack token missing");
    return "";
  }

  WiFiClientSecure client;
  client.setInsecure(); // For simplicity, no SSL validation

  if (!client.connect("slack.com", 443)) {
    Serial.println("Connection failed to Slack");
    return "";
  }

  String url = endpoint;
  if (userId != "") {
    url += "?user=" + userId;
  }
  
  client.printf("GET %s HTTP/1.1\r\nHost: slack.com\r\nAuthorization: Bearer %s\r\nConnection: close\r\n\r\n",
                url.c_str(), slackToken.c_str());

  // Skip headers
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") break;
  }
  
  // Read response body
  String payload;
  while (client.available()) {
    payload += client.readString();
  }
  
  client.stop();
  return payload;
}

// Fetch user status using the configured user ID
SlackUserStatus fetchSlackUserStatus() {
  if (slackUserId == "") {
    Serial.println("Slack user ID not configured");
    SlackUserStatus status;
    return status;
  }
  return fetchSlackUserStatus(slackUserId);
}

// Fetch user status for a specific user ID
SlackUserStatus fetchSlackUserStatus(const String& userId) {
  SlackUserStatus userStatus;
  
  if (userId == "" || slackToken == "") {
    Serial.println("Slack config missing");
    return userStatus;
  }

  // Get user profile information
  String profilePayload = makeSlackRequest("/api/users.profile.get", userId);
  if (profilePayload == "") {
    Serial.println("Failed to fetch user profile");
    return userStatus;
  }

  // Get user presence information
  String presencePayload = makeSlackRequest("/api/users.getPresence", userId);
  
  // Check if the API call was successful
  if (profilePayload.indexOf("\"ok\":true") == -1) {
    Serial.println("Slack API error in profile response");
    Serial.println("Response: " + profilePayload.substring(0, 200));
    return userStatus;
  }

  // Extract user information from profile response
  userStatus.userId = userId;
  userStatus.displayName = extractJsonString(profilePayload, "display_name");
  userStatus.realName = extractJsonString(profilePayload, "real_name");
  userStatus.statusText = extractJsonString(profilePayload, "status_text");
  userStatus.statusEmoji = extractJsonString(profilePayload, "status_emoji");
  userStatus.statusExpiration = extractJsonString(profilePayload, "status_expiration");
  
  // Extract presence information if available
  if (presencePayload != "" && presencePayload.indexOf("\"ok\":true") != -1) {
    String presence = extractJsonString(presencePayload, "presence");
    userStatus.isOnline = (presence == "active");
  }
  
  userStatus.isValid = true;
  
  // Debug output
  Serial.println("=== Slack User Status ===");
  Serial.println("User ID: " + userStatus.userId);
  Serial.println("Display Name: " + userStatus.displayName);
  Serial.println("Real Name: " + userStatus.realName);
  Serial.println("Status Text: " + userStatus.statusText);
  Serial.println("Status Emoji: " + userStatus.statusEmoji);
  Serial.println("Is Online: " + String(userStatus.isOnline ? "Yes" : "No"));
  Serial.println("========================");
  
  return userStatus;
}

// Legacy function for backward compatibility
String fetchSlackStatus() {
  SlackUserStatus status = fetchSlackUserStatus();
  return status.statusText;
}
