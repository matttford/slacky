#ifndef IMAGE_TEST_H
#define IMAGE_TEST_H

#include <Arduino.h>
#include <Adafruit_SSD1351.h>
#include <SPIFFS.h>
#include <math.h>

void testImageDisplay(Adafruit_SSD1351& display);
void drawTestPattern(Adafruit_SSD1351& display);
void testSPIFFS(Adafruit_SSD1351& display);
void listDir(File dir, String indent, int level);
void displayRGB565File(Adafruit_SSD1351& display, String filePath);

#endif