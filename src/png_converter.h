#ifndef PNG_CONVERTER_H
#define PNG_CONVERTER_H

#include <Arduino.h>

// Structure to hold RGB565 image data
struct RGB565Image {
    uint16_t* data;
    int width;
    int height;
    size_t dataSize;
};

// Convert PNG file to RGB565 format
RGB565Image loadPNGAsRGB565(const char* filename);

// Free memory allocated for RGB565 image
void freeRGB565Image(RGB565Image& img);

// Convert RGB888 to RGB565
uint16_t rgb888ToRgb565(uint8_t r, uint8_t g, uint8_t b);

#endif