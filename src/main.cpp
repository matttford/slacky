#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1351.h>
#include <SPI.h>
#include <SPIFFS.h>

// Include RGB565 image headers
#include "assets/emojis_rgb565/vacation.h"
#include "assets/emojis_rgb565/busy_no_interrupting.h"
#include "assets/emojis_rgb565/busy_interrupt_ok.h"
#include "assets/emojis_rgb565/busy_interrupt_maybe.h"
#include "assets/emojis_rgb565/busy_stay_clear.h"

// Pin definitions for XIAO ESP32S3 SPI
#define PIN_MOSI D10  // GPIO9 -> DIN (MOSI)
#define PIN_SCLK D9   // GPIO7 -> CLK (SCK) 
#define PIN_CS   D8   // GPIO1 -> CS
#define PIN_DC   D7   // GPIO2 -> DC
#define PIN_RST  D6   // GPIO3 -> RST

// Color definitions
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define WHITE   0xFFFF

// OLED dimensions
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 128

#define IMG_WIDTH 128
#define IMG_HEIGHT 128


// Create OLED object (128x128 SSD1351)
Adafruit_SSD1351 display = Adafruit_SSD1351(SCREEN_WIDTH, SCREEN_HEIGHT, PIN_CS, PIN_DC, PIN_MOSI, PIN_SCLK, PIN_RST);



static const uint16_t imageData[] PROGMEM = {};
 
void setup(void) {
  Serial.begin(9600);
  Serial.println("ESP32 is starting up");
 
  Serial.println("TFT screen initialising");
  display.begin();
 
  //Uncomment this if you want to rotate the screen
  //display.setRotation(1);
 
  //Call this to clear any previous images
  display.fillScreen(BLACK);
  
 
  //This function renders the image data onto the display
  //Parameters: xPos, yPos, bitmap data, imageWidth, imageHeight
  display.drawRGBBitmap(0, 0, busy_interrupt_maybe, BUSY_INTERRUPT_MAYBE_WIDTH, BUSY_INTERRUPT_MAYBE_HEIGHT);
  Serial.println("Busy interrupt maybe emoji has been rendered onto screen");
}


void loop() {}