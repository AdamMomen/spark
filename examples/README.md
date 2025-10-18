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

1. Start with `basic_led` to test hardware
2. Move to `web_control` for API development
3. Use `dashboard_template` for the challenge
4. Explore `ota_update` for advanced features
