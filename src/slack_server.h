#ifndef SLACK_SERVER_H
#define SLACK_SERVER_H

#include <WiFi.h>
#include <WebServer.h>
#include <Preferences.h>

extern String slackUserId;
extern String slackToken;

void startSlackServer();
void handleSlackServer();
void saveSlackConfig();
void loadSlackConfig();
void handleStatusQuery();

#endif
