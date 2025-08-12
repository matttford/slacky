#include "slack_server.h"
#include "slack_status.h"

WebServer server(80);

String slackUserId = "";
String slackToken = ""; // Shared token from Slack

// Handle root page
void handleRoot() {
  String html = "<html><body>"
                "<h1>Slack Configuration</h1>"
                "<form action='/save' method='POST'>"
                "Slack User ID: <input type='text' name='userid' value='" + slackUserId + "'><br><br>"
                "<input type='submit' value='Save'>"
                "</form>"
                "<p>Current User ID: " + slackUserId + "</p>"
                "<hr>"
                "<h2>Query User Status</h2>"
                "<form action='/status' method='GET'>"
                "User ID to query: <input type='text' name='user' placeholder='Enter user ID'><br><br>"
                "<input type='submit' value='Get Status'>"
                "</form>"
                "</body></html>";
  server.send(200, "text/html", html);
}

// Handle save request
void handleSave() {
  if (server.hasArg("userid")) {
    slackUserId = server.arg("userid");
    saveSlackConfig();
    server.send(200, "text/html", 
      "<html><body>"
      "<h1>Configuration Saved!</h1>"
      "<p>User ID: " + slackUserId + "</p>"
      "<a href='/'>Back to Config</a>"
      "</body></html>");
    Serial.println("Saved User ID: " + slackUserId);
  } else {
    server.send(400, "text/plain", "Missing userid parameter");
  }
}

// Handle status query
void handleStatusQuery() {
  if (!server.hasArg("user")) {
    server.send(400, "text/html", 
      "<html><body>"
      "<h1>Error</h1>"
      "<p>No user ID provided</p>"
      "<a href='/'>Back to Config</a>"
      "</body></html>");
    return;
  }
  
  String queryUserId = server.arg("user");
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
  server.on("/save", HTTP_POST, handleSave);
  server.on("/status", HTTP_GET, handleStatusQuery);
  server.begin();
  Serial.println("Slack web server started!");
}

void handleSlackServer() {
  server.handleClient();  // Handle incoming requests
}

void saveSlackConfig() {
  Preferences prefs;
  prefs.begin("slack", false);
  prefs.putString("userid", slackUserId);
  prefs.end();
}

void loadSlackConfig() {
  Preferences prefs;
  prefs.begin("slack", true);
  slackUserId = prefs.getString("userid", "");
  prefs.end();
}
