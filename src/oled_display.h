#ifndef OLED_DISPLAY_H
#define OLED_DISPLAY_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1351.h>
#include "slack_status.h"
#include "image_display.h"

// Pin definitions for XIAO ESP32S3 SPI (from example.cpp)
#define PIN_MOSI D10  // GPIO9 -> DIN (MOSI) - Hardware SPI - Master Out Slave In
#define PIN_SCLK D9   // GPIO7 -> CLK (SCK) - Hardware SPI - Clock
#define PIN_CS   D8   // GPIO1 -> CS - Chip Select
#define PIN_DC   D7   // GPIO2 -> DC - Data/Command
#define PIN_RST  D6   // GPIO3 -> RST - Reset

// OLED dimensions
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 128

// Color definitions (16-bit RGB565)
#define COLOR_BLACK   0x0000
#define COLOR_WHITE   0xFFFF
#define COLOR_RED     0xF800
#define COLOR_GREEN   0x07E0
#define COLOR_BLUE    0x001F
#define COLOR_CYAN    0x07FF
#define COLOR_MAGENTA 0xF81F
#define COLOR_YELLOW  0xFFE0
#define COLOR_ORANGE  0xFD20
#define COLOR_PURPLE  0x8010


// Function prototypes
void initOLED();
void displaySlackStatus(const SlackUserStatus& status, const String& ipAddress = "");
void displayConnectionStatus(const String& message, uint16_t color);
void displayConfigMode(const String& ipAddress = "192.168.4.1");
void displayWiFiConnecting();
void displayWiFiConnected(const String& ipAddress);
void displayAPMode(const String& apName, const String& ipAddress);
void displayWiFiLoading();
uint16_t getStatusColor(const String& statusText);
void drawStatusBackground(uint16_t color);

#endif