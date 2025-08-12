#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1351.h>

// ==== Function prototypes ====
void drawRainbowSpiral();
void drawColorWaves();
void drawPulsatingCircles();
void drawMatrixRain();
void drawFireEffect();
void drawAuroraEffect();

// ==== Pin definitions for your wiring ====
// Using correct D pin constants for XIAO ESP32S3 SPI
#define PIN_MOSI D10  // GPIO9 -> DIN (MOSI) - Hardware SPI - Master Out Slave In
#define PIN_SCLK D9   // GPIO7 -> CLK (SCK) - Hardware SPI - Clock
#define PIN_CS   D8   // GPIO1 -> CS - Chip Select
#define PIN_DC   D7   // GPIO2 -> DC - Data/Command
#define PIN_RST  D6   // GPIO3 -> RST - Reset

// ==== OLED dimensions ====
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 128

// ==== Color definitions (16-bit RGB565) ====
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


// Create OLED object (using SPI constructor)
Adafruit_SSD1351 display = Adafruit_SSD1351(
    SCREEN_WIDTH,
    SCREEN_HEIGHT,
    PIN_CS,
    PIN_DC,
    PIN_MOSI,
    PIN_SCLK,
    PIN_RST
);


// ==== IMPRESSIVE RGB VISUAL EFFECTS ====

// Helper function to create HSV to RGB color conversion
uint16_t HSVtoRGB565(float h, float s, float v) {
  float c = v * s;
  float x = c * (1 - abs(fmod(h / 60.0, 2) - 1));
  float m = v - c;
  
  float r, g, b;
  if (h >= 0 && h < 60) { r = c; g = x; b = 0; }
  else if (h >= 60 && h < 120) { r = x; g = c; b = 0; }
  else if (h >= 120 && h < 180) { r = 0; g = c; b = x; }
  else if (h >= 180 && h < 240) { r = 0; g = x; b = c; }
  else if (h >= 240 && h < 300) { r = x; g = 0; b = c; }
  else { r = c; g = 0; b = x; }
  
  uint8_t red = (r + m) * 255;
  uint8_t green = (g + m) * 255;
  uint8_t blue = (b + m) * 255;
  
  return display.color565(red, green, blue);
}


void setup() {
  Serial.begin(115200);
  Serial.println("Starting OLED test...");

  // Set up SPI pins (some boards require this)
  SPI.begin(PIN_SCLK, -1, PIN_MOSI, PIN_CS);
  // Initialize OLED
  display.begin();
  Serial.println("OLED initialized!");
  
  // Clear screen and show startup message
  display.fillScreen(COLOR_BLACK);
  display.setTextColor(COLOR_CYAN);
  display.setTextSize(2);
  display.setCursor(10, 50);
  display.println("RGB VISUAL");
  display.setCursor(15, 70);
  display.println("EFFECTS");
  
  delay(2000);  // Show startup message
}

void loop() {
  static int effect_mode = 0;
  static unsigned long last_change = 0;
  const unsigned long change_interval = 8000;  // Change effect every 8 seconds
  
  if (millis() - last_change > change_interval) {
    effect_mode = (effect_mode + 1) % 6;
    last_change = millis();
    
    Serial.print("Switching to effect: ");
    Serial.println(effect_mode);
  }
  
  switch (effect_mode) {
    case 0:
      drawRainbowSpiral();
      break;
      
    case 1:
      drawColorWaves();
      break;
      
    case 2:
      drawPulsatingCircles();
      break;
      
    case 3:
      drawMatrixRain();
      break;
      
    case 4:
      drawFireEffect();
      break;
      
    case 5:
      drawAuroraEffect();
      break;
  }
  
  delay(30);  // Smooth 30fps animation
}

// ==== SPECTACULAR VISUAL EFFECTS ====

// Effect 1: Rainbow Spiral
void drawRainbowSpiral() {
  static float rotation = 0;
  rotation += 2.0;
  
  int centerX = SCREEN_WIDTH / 2;
  int centerY = SCREEN_HEIGHT / 2;
  
  for (int y = 0; y < SCREEN_HEIGHT; y++) {
    for (int x = 0; x < SCREEN_WIDTH; x++) {
      float dx = x - centerX;
      float dy = y - centerY;
      float distance = sqrt(dx * dx + dy * dy);
      float angle = atan2(dy, dx) * 180.0 / PI;
      
      float hue = fmod(angle + rotation + distance * 3, 360);
      if (hue < 0) hue += 360;
      
      float brightness = (sin(distance * 0.1 - rotation * 0.05) + 1) * 0.5;
      uint16_t color = HSVtoRGB565(hue, 1.0, brightness);
      
      display.drawPixel(x, y, color);
    }
  }
}

// Effect 2: Color Waves
void drawColorWaves() {
  static float time = 0;
  time += 0.15;
  
  for (int y = 0; y < SCREEN_HEIGHT; y++) {
    for (int x = 0; x < SCREEN_WIDTH; x++) {
      float wave1 = sin((x * 0.05) + time) * 0.5 + 0.5;
      float wave2 = sin((y * 0.05) + time * 1.3) * 0.5 + 0.5;
      float wave3 = sin(((x + y) * 0.03) + time * 0.8) * 0.5 + 0.5;
      
      uint8_t red = wave1 * 255;
      uint8_t green = wave2 * 255;
      uint8_t blue = wave3 * 255;
      
      uint16_t color = display.color565(red, green, blue);
      display.drawPixel(x, y, color);
    }
  }
}

// Effect 3: Pulsating Circles
void drawPulsatingCircles() {
  static float pulse = 0;
  pulse += 0.2;
  
  display.fillScreen(COLOR_BLACK);
  
  int centerX = SCREEN_WIDTH / 2;
  int centerY = SCREEN_HEIGHT / 2;
  
  for (int i = 0; i < 5; i++) {
    float radius = 10 + i * 15 + sin(pulse + i) * 8;
    float hue = fmod(pulse * 20 + i * 60, 360);
    uint16_t color = HSVtoRGB565(hue, 1.0, 0.8);
    
    display.drawCircle(centerX, centerY, radius, color);
    if (radius > 3) {
      display.drawCircle(centerX, centerY, radius - 1, color);
    }
  }
}

// Effect 4: Matrix Rain
void drawMatrixRain() {
  static uint8_t matrix[SCREEN_WIDTH];
  static bool initialized = false;
  static float time = 0;
  time += 0.1;
  
  if (!initialized) {
    for (int i = 0; i < SCREEN_WIDTH; i++) {
      matrix[i] = random(SCREEN_HEIGHT);
    }
    initialized = true;
  }
  
  // Clear screen for fresh frame
  display.fillScreen(COLOR_BLACK);
  
  // Draw matrix drops with trails
  for (int x = 0; x < SCREEN_WIDTH; x++) {
    // Draw trail (multiple pixels above current position)
    for (int trail = 0; trail < 8; trail++) {
      int y = matrix[x] - trail;
      if (y >= 0 && y < SCREEN_HEIGHT) {
        float brightness = 1.0 - (trail / 8.0);
        brightness = brightness * brightness;  // Square for better falloff
        uint16_t color = HSVtoRGB565(120, 1.0, brightness);  // Green
        display.drawPixel(x, y, color);
      }
    }
    
    // Move drop down
    matrix[x]++;
    
    // Reset if off screen
    if (matrix[x] > SCREEN_HEIGHT + 8) {
      matrix[x] = random(SCREEN_HEIGHT / 2);  // Random restart position
    }
    
    // Random new drops
    if (random(150) < 2) {  // 1.3% chance
      matrix[x] = 0;
    }
  }
}

// Effect 5: Fire Effect
void drawFireEffect() {
  static float time = 0;
  time += 0.1;
  
  for (int y = 0; y < SCREEN_HEIGHT; y++) {
    for (int x = 0; x < SCREEN_WIDTH; x++) {
      float flame = 0;
      
      // Create fire-like noise
      flame += sin(x * 0.1 + time * 2) * 0.3;
      flame += sin(x * 0.05 + time * 1.5) * 0.2;
      flame += sin((x + y) * 0.08 + time) * 0.1;
      
      // Make fire rise from bottom
      float height_factor = 1.0 - (y / (float)SCREEN_HEIGHT);
      flame *= height_factor * height_factor;
      flame += height_factor * 0.3;
      
      // Clamp and convert to fire colors
      flame = constrain(flame, 0, 1);
      
      uint8_t red = flame * 255;
      uint8_t green = flame * flame * 200;  // Less green
      uint8_t blue = flame * flame * flame * 100;  // Even less blue
      
      uint16_t color = display.color565(red, green, blue);
      display.drawPixel(x, y, color);
    }
  }
}

// Effect 6: Aurora Effect
void drawAuroraEffect() {
  static float time = 0;
  time += 0.08;
  
  for (int y = 0; y < SCREEN_HEIGHT; y++) {
    for (int x = 0; x < SCREEN_WIDTH; x++) {
      float wave1 = sin((x * 0.02) + time) * 20;
      float wave2 = sin((x * 0.03) + time * 1.4) * 15;
      float wave3 = sin((x * 0.015) + time * 0.8) * 25;
      
      float aurora_y = SCREEN_HEIGHT * 0.3 + wave1 + wave2 + wave3;
      float distance = abs(y - aurora_y);
      
      if (distance < 30) {
        float intensity = (30 - distance) / 30.0;
        intensity = intensity * intensity;  // Square for sharper falloff
        
        float hue = fmod(x * 2 + time * 30, 360);
        if (hue > 180) hue = 360 - hue;  // Focus on blue-green spectrum
        hue = 120 + hue * 0.5;  // Shift to aurora colors
        
        uint16_t color = HSVtoRGB565(hue, 0.8, intensity);
        display.drawPixel(x, y, color);
      } else {
        display.drawPixel(x, y, COLOR_BLACK);
      }
    }
  }
}
