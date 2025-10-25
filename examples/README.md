# Examples Directory

This directory contains example code for the IoT workshop.

## Examples

### 1. Basic LED Blink (`basic_led/`)
Simple LED blink example to test hardware connections.

**Features:**
- Red LED (D2) blink
- Serial output
- Basic pin configuration

**Usage:**
```bash
pio run -e nodemcuv2 -t upload
```

### 2. Web Control (`web_control/`)
Web server with LED control API endpoints.

**Features:**
- REST API endpoints
- HTML dashboard
- LED control via HTTP
- System status API

**Usage:**
```bash
pio run -e nodemcuv2 -t upload
```

### 3. Dashboard Template (`dashboard_template/`)
Starting template for the creative dashboard challenge.

**Features:**
- Mobile-responsive design
- Real-time status updates
- LED control interface
- Customizable styling

**Usage:**
```bash
pio run -e nodemcuv2 -t upload
```

### 4. OTA Update (`ota_update/`)
Example showing Over-The-Air update functionality.

**Features:**
- OTA update setup
- Password protection
- Update progress monitoring

**Usage:**
```bash
pio run -e nodemcuv2 -t upload
```

### 5. ASCII Animation (`ascii_animation/`)
Cool startup animation for OLED and LEDs.

**Features:**
- Screen-friendly animations
- Team welcome messages
- System status display
- LED patterns

**Usage:**
```bash
pio run -e nodemcuv2 -t upload
```

### 6. OLED Test (`oled_test/`)
OLED display test with custom I2C pins.

**Features:**
- Custom I2C pin configuration (D5=SCL, D6=SDA)
- Display test with debug output
- Blinking indicator
- Wiring verification

**Usage:**
```bash
pio run -e nodemcuv2 -t upload
```

### 7. Power Test (`power_test/`)
Basic power and connection test.

**Features:**
- Built-in LED test
- I2C pin state checking
- Wiring verification
- Power diagnostics

**Usage:**
```bash
pio run -e nodemcuv2 -t upload
```

### 8. I2C Scanner (`i2c_scanner/`)
I2C device detection and scanning.

**Features:**
- I2C device discovery
- Address scanning
- Device identification
- Continuous monitoring

**Usage:**
```bash
pio run -e nodemcuv2 -t upload
```

### 9. Potentiometer Control (`potentiometer_control/`)
Real-time potentiometer control with WebSocket communication.

**Features:**
- WebSocket real-time updates
- Dynamic background color changes
- Combined LED + potentiometer interface
- Voltage display and ADC conversion
- Mobile-responsive design

**Usage:**
```bash
pio run -e nodemcuv2 -t upload
```

## How to Use Examples

1. Copy example code to `src/main.cpp`
2. Update WiFi credentials in `platformio.ini`
3. Update team information in `main.cpp`
4. Compile and upload:
   ```bash
   pio run -e nodemcuv2 -t upload
   ```

## Customization

Each example can be customized for your team:
- Change team name and member names
- Modify LED patterns
- Add custom features
- Update dashboard styling

## Next Steps

**If OLED not working:**
1. Start with `power_test` to check basic connections
2. Use `i2c_scanner` to detect OLED on I2C bus
3. Try `oled_test` to verify display functionality

**If OLED working:**
1. Try `basic_led` to test LED connections
2. Move to `web_control` for API development
3. Try `potentiometer_control` for real-time sensor data
4. Use `dashboard_template` for the challenge
5. Explore `ascii_animation` for cool effects
6. Learn `ota_update` for advanced features
