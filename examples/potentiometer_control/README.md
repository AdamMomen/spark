# Potentiometer Control Example

This example demonstrates real-time potentiometer control with WebSocket communication, building on the concepts from Lecture 2.

## Features

- **Passwordless WiFi**: Creates "IoT-Workshop" Access Point (no router needed)
- **Real-time Updates**: WebSocket communication for instant potentiometer value updates
- **Modern UI**: Glassmorphism design with animated potentiometer gauge
- **LED Control**: Combined LED control with potentiometer reading
- **Dynamic Background**: HSL color changes based on potentiometer value
- **Mobile Responsive**: Touch-friendly interface for all devices

## Hardware Setup

### Circuit Connections
```
ESP8266 Board:
    3.3V ----[1kΩ Potentiometer]----GND
              |
              A0 (ADC Input)

Red LED:   D2 ----[330Ω Resistor]----[LED]----GND
Green LED: D3 ----[330Ω Resistor]----[LED]----GND
```

### Components Required
- ESP8266MOD board
- 1kΩ potentiometer
- 2x LEDs (Red and Green)
- 2x 330Ω resistors
- Breadboard and jumper wires

## Usage

### Pre-Workshop Setup
1. Update team information in the code:
   ```cpp
   const char* TEAM_NAME = "Team A";
   const char* MEMBER_1 = "Alice";
   const char* MEMBER_2 = "Bob";
   ```

2. Compile and upload:
   ```bash
   pio run -e nodemcuv2 -t upload
   ```

### Workshop Day
1. **Connect hardware** as shown in the circuit diagram
2. **Power on** the ESP8266 board
3. **Open Serial Monitor** (115200 baud) to see:
   ```
   Access Point created!
   AP IP address: 192.168.4.1
   Connect to: IoT-Workshop
   No password required!
   ```
4. **Connect your device** to WiFi network "IoT-Workshop" (no password)
5. **Open browser** and go to `http://192.168.4.1`
6. **Turn the potentiometer** to see:
   - Animated gauge needle rotation
   - Dynamic HSL background colors
   - Real-time ADC value updates
7. **Test LED controls** while potentiometer is running

## Web Interface Features

### Real-time Potentiometer Display
- **Animated Gauge**: Circular potentiometer with rotating needle
- **ADC Value**: Large display of raw analog reading (0-1023)
- **Dynamic Background**: HSL color changes based on potentiometer value
- **Update Rate**: 100ms intervals via WebSocket

### LED Control Panel
- **Modern Buttons**: Gradient buttons with hover effects
- **Real-time Status**: Visual indicators with color coding
- **Smooth Animations**: Glassmorphism effects and transitions

### Access Point Features
- **Network Name**: "IoT-Workshop" (no password required)
- **IP Address**: Always 192.168.4.1
- **Multiple Devices**: Supports multiple simultaneous connections
- **Workshop Ready**: No router or venue WiFi needed

## Technical Details

### WebSocket Communication
- **Port**: 81
- **Protocol**: WebSocket
- **Data Format**: JSON
- **Update Frequency**: 100ms
- **Auto-reconnect**: Built-in reconnection handling

### API Endpoints
- `GET /` - Main dashboard page
- `GET /api/status` - System status JSON
- `POST /api/led/red/toggle` - Toggle red LED
- `POST /api/led/green/toggle` - Toggle green LED
- `GET /api/pot/read` - Read potentiometer value

### ADC Conversion
```cpp
int potValue = analogRead(A0);           // Read raw ADC value (0-1023)
float voltage = (potValue / 1023.0) * 3.3;  // Convert to voltage (0-3.3V)
```

## Troubleshooting

### Potentiometer Not Working
- Check all three connections (3.3V, A0, GND)
- Verify potentiometer orientation
- Test with multimeter for voltage changes
- Check for loose breadboard connections

### Access Point Connection Issues
- Check if "IoT-Workshop" network appears in WiFi settings
- Ensure ESP8266 is powered on and code uploaded
- Try disconnecting and reconnecting to the network
- Check Serial Monitor for "Access Point created!" message
- Verify IP address is 192.168.4.1

### WebSocket Connection Failed
- Verify WebSocket port 81 is open
- Check browser console for errors
- Ensure connected to "IoT-Workshop" network
- Try refreshing the page
- Check if multiple devices are connected to same ESP8266

### Background Color Not Changing
- Check browser JavaScript console
- Verify WebSocket messages are received
- Test with different browsers
- Check JSON parsing

### Erratic Readings
- Check for loose connections
- Verify 3.3V power supply stability
- Test potentiometer with multimeter
- Add small capacitor for noise filtering

## Learning Objectives Achieved

✅ **Potentiometer Basics**: Understanding variable resistors and voltage dividers  
✅ **analogRead() Function**: Master ADC conversion and voltage calculation  
✅ **WebSocket Communication**: Real-time bidirectional communication  
✅ **Combined Interface**: LED control + potentiometer reading on same page  
✅ **Circuit Testing**: Multimeter verification and troubleshooting  

## Next Steps

- Read [lect2.md](../../docs/lect2.md) for detailed explanations
- Try [dashboard_template](../dashboard_template/) for the creative challenge
- Experiment with different sensor types
- Learn about data logging and visualization
