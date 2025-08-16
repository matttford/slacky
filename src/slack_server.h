#ifndef SLACK_SERVER_H
#define SLACK_SERVER_H

#include <WiFi.h>
#include <WebServer.h>
#include <Preferences.h>
#include <DNSServer.h>

extern String slackUserId;
extern String slackToken;
extern String wifiSSID;
extern String wifiPassword;

void startSlackServer();
void handleSlackServer();
void saveSlackConfig();
void loadSlackConfig();
void saveWiFiConfig();
void loadWiFiConfig();
void handleStatusQuery();
bool startAPMode();
bool connectToWiFi();
void handleDNSServer();

#endif
