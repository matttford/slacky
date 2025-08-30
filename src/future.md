---

### `FUTURE_PLANS.md`

```markdown
# Future Plans for Slack Status OLED

## Configuration & UX

- **User Selection**
  - Allow the device to list all Slack users in a workspace (via API).
  - Enable selecting a user by name instead of hardcoding the User ID.
  - Save the chosen User ID in non-volatile storage.

- **Wi-Fi Configuration**
  - Replace hardcoded SSID/password with configurable setup.
  - Store Wi-Fi credentials in flash (NVS) for persistence.

- **Slack Token Configuration**
  - Token should not be hardcoded in firmware.
  - Provide a local web configuration page to input and save the Slack Bot Token
    securely.

---

## Display & Images

- **Emoji Rendering**
  - Since direct emoji rendering is not supported, manually convert Slack emoji
    → RGB bitmaps.
  - Store a set of commonly used Slack emoji images in memory (SPI flash /
    external storage).
  - Map Slack emoji codes to these images for display.

- **Animated Status**
  - Support simple multi-frame animations (converted GIFs or sprite sheets).
  - Display cycling images for fun/emphasis.

---

## Hardware Expansion

- **Enclosure**
  - Design and 3D print a custom case.
  - Integrate button(s) for cycling users or refreshing manually.
  - Consider a stand or magnetic mount for desk usability.

- **Power System**
  - Add rechargeable Li-ion/LiPo battery.
  - Charging circuit via USB-C.
  - Battery management with power-saving features (deep sleep, display off when
    idle).

- **Additional Indicators**
  - RGB LED synced with status color (green = available, red = busy, yellow =
    away, etc.).
  - Optional buzzer for notifications.

---

## Long-Term Ideas

- Show status text alongside emoji (scrolling text on OLED).
```


Current Status: 
- The Wifi and slack credentials are configurable and stored in the flash.
- OLED display is working and able to display the RGB bitmaps.

Still Needed: 
- The OLED Should display the current status
  - Things like: wifi connecting, wifi connected, fetching slack status, slack status fetched, error, etc.
- Display the Slack status on the OLED display.
 - The slack status png should be mapped to the RGB565 bitmaps.
 - The OLED should display the slack status image and the slack status text. The text should be scrolled. 