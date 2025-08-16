# Slack Status OLED Project

## Overview

This project uses a **Waveshare 1.5" OLED display** connected to a **Seeed
Studio XIAO ESP32-S3** to display a Slack user’s current status.\
The device connects to Wi-Fi, queries Slack’s API for a user’s status (emoji +
text), and displays the corresponding image or symbol on the OLED.\
The goal is to create a compact IoT device that sits on a desk and gives a
real-time visual indicator of Slack presence.

---

## Purpose

- Provide a **physical, at-a-glance indicator** of a Slack user’s current
  status.
- Enable teammates or family members to see whether the user is busy, available,
  or away without checking a screen.
- Extend Slack’s digital workspace into the physical world with a fun and useful
  hardware integration.

---

## Hardware

- **Seeed Studio XIAO ESP32-S3**
  - Dual-core Xtensa LX7 MCU
  - Wi-Fi + Bluetooth
  - 8 MB PSRAM, 8 MB Flash
- **Waveshare 1.5" 128x128 OLED Display** (SPI)
- **RGB LED indicator** (optional, for quick status color mapping)
- **USB-C Power / Battery** (to be finalized)

### Pinout (ESP32-S3 -> OLED)

```c
// Using correct D pin constants for XIAO ESP32S3 SPI
#define PIN_MOSI D10  // GPIO9 -> DIN (MOSI) - Hardware SPI - Master Out Slave In
#define PIN_SCLK D9   // GPIO7 -> CLK (SCK) - Hardware SPI - Clock
#define PIN_CS   D8   // GPIO1 -> CS - Chip Select
#define PIN_DC   D7   // GPIO2 -> DC - Data/Command
#define PIN_RST  D6   // GPIO3 -> RST - Reset
```
