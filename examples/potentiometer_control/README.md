# Potentiometer Control Example

This example demonstrates real-time potentiometer control with WebSocket communication, building on the concepts from Lecture 2.

## Features

- **Real-time Updates**: WebSocket communication for instant potentiometer value updates
- **Background Color**: Dynamic background color changes based on potentiometer value (Red to Blue gradient)
- **LED Control**: Combined LED control with potentiometer reading
- **Voltage Display**: Shows both ADC value (0-1023) and calculated voltage (0-3.3V)
- **System Status**: Real-time system information display
- **Mobile Responsive**: Works on desktop and mobile devices

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

2. Update WiFi credentials:
   ```cpp
   const char* WIFI_SSID = "WorkshopWiFi";
   const char* WIFI_PASSWORD = "workshop2025";
   ```

3. Compile and upload:
   ```bash
   pio run -e nodemcuv2 -t upload
   ```

### Workshop Day
1. Connect the potentiometer and LEDs as shown in the circuit diagram
2. Power on the ESP8266 board
3. Open Serial Monitor (115200 baud) to see connection status
4. Open browser and go to the IP address shown in Serial Monitor
5. Turn the potentiometer to see:
   - Real-time background color changes
   - ADC value updates (0-1023)
   - Voltage readings (0-3.3V)
6. Test LED controls while potentiometer is running

## Web Interface Features

### Real-time Potentiometer Display
- **ADC Value**: Shows raw analog reading (0-1023)
- **Voltage**: Calculated voltage (0-3.3V)
- **Background Color**: Smooth color transition from red to blue
- **Update Rate**: 100ms intervals via WebSocket

### LED Control Panel
- **Red LED Toggle**: Turn red LED on/off
- **Green LED Toggle**: Turn green LED on/off
- **Status Display**: Shows current LED states

### System Information
- **WiFi Status**: Connection status
- **Uptime**: System running time
- **Free Memory**: Available RAM
- **LED States**: Current LED status

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

### WebSocket Connection Failed
- Verify WebSocket port 81 is open
- Check browser console for errors
- Ensure same network as ESP8266
- Try refreshing the page

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
