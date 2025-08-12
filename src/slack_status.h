#ifndef SLACK_STATUS_H
#define SLACK_STATUS_H

#include <WiFiClientSecure.h>
#include "slack_server.h"

// Structure to hold Slack user status information
struct SlackUserStatus {
  String userId;
  String displayName;
  String realName;
  String statusText;
  String statusEmoji;
  String statusExpiration;
  bool isOnline;
  bool isValid;
  
  // Constructor
  SlackUserStatus() : isOnline(false), isValid(false) {}
};

SlackUserStatus fetchSlackUserStatus();
SlackUserStatus fetchSlackUserStatus(const String& userId);

#endif
