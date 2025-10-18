# ASCII Animation Example

This example creates a cool startup animation using the WorkshopESP library.

## Features

- **OLED Animation**: Cool animations designed for 0.96" OLED screen (128x64)
- **Team Welcome**: Displays team name and member names
- **LED Patterns**: Cool LED light shows
- **System Status**: Shows system readiness
- **Screen-Friendly**: All text fits perfectly on small display

## Usage

### Pre-Workshop Setup
1. Update team information in the code:
   ```cpp
   const char* TEAM_NAME = "Team A";
   const char* MEMBER_1 = "Alice";
   const char* MEMBER_2 = "Bob";
   ```

2. Update WiFi credentials:
   ```cpp
   const char* WIFI_SSID = "WorkshopWiFi";
   const char* WIFI_PASSWORD = "workshop2025";
   ```

3. Compile and upload to each board:
   ```bash
   pio run -e nodemcuv2 -t upload
   ```

### Workshop Day
1. Plug in power to the boards
2. Watch the cool animation sequence
3. Teams will see their names and welcome messages
4. System automatically starts web server after animation

## Animation Sequence

1. **OLED Animation**: Displays workshop info in screen-friendly format
2. **Team Welcome**: Shows team name and member names
3. **LED Patterns**: 
   - Alternating red/green blink
   - Both LEDs blink together
   - Chase effect
4. **System Status**: Shows WiFi, IP, API, and Dashboard status
5. **Final Welcome**: Team-specific welcome animation

## Customization

### Different Teams
Create different versions for each team:
- Team A: Alice & Bob
- Team B: Charlie & Diana
- Team C: Eve & Frank
- Team D: Grace & Henry

### Custom Messages
Add your own welcome messages:
```cpp
const char* customMessages[] = {
  "Welcome to",
  "IoT Workshop!",
  "Let's build",
  "something",
  "amazing!"
};
```

### LED Patterns
Create custom LED patterns:
```cpp
void customPattern() {
  // Your custom pattern here
  for (int i = 0; i < 10; i++) {
    workshop.setLED(1, true);
    delay(100);
    workshop.setLED(1, false);
    delay(100);
  }
}
```

## Troubleshooting

- **No display**: Check OLED connections
- **No LEDs**: Verify LED circuit connections
- **No WiFi**: Check WiFi credentials
- **Animation stuck**: Restart the board
