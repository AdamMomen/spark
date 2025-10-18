# Troubleshooting Guide

## Common Issues and Solutions

### Serial Monitor Problems

**Issue**: Serial monitor shows gibberish or no output
**Solution**: 
- Check baud rate is set to 115200
- Verify correct port selection
- Try different USB cable
- Restart Cursor IDE

**Issue**: "Port not found" error
**Solution**:
- Check `pio device list` for available ports
- Try different USB port
- Install CH340 drivers: `brew install --cask wch-ch34x-usb-serial-driver`
- Check device permissions: `sudo chmod 666 /dev/tty.usbserial-*`

### Upload Issues

**Issue**: Upload fails with "device not found"
**Solution**:
1. Hold RESET button
2. Press and hold BOOT button
3. Release RESET button
4. Release BOOT button
5. Try upload again

**Issue**: Upload timeout
**Solution**:
- Reduce upload speed in `platformio.ini`
- Try different USB cable
- Check power supply (use external power if needed)

### WiFi Connection Problems

**Issue**: ESP8266 won't connect to WiFi
**Solution**:
- Verify SSID and password in `platformio.ini`
- Ensure 2.4GHz network (ESP8266 doesn't support 5GHz)
- Check WiFi signal strength
- Try different network or your phone hotspot

**Issue**: "WiFi connection failed"
**Solution**:
- Check network credentials
- Verify network is not hidden
- Try open network for testing
- Check router settings

### LED Circuit Issues

**Issue**: LED not lighting up
**Solution**:
- Check LED polarity (longer leg = positive)
- Verify resistor connection (220Ω)
- Test with multimeter
- Check GPIO pin assignment
- Ensure proper ground connection

**Issue**: LED very dim
**Solution**:
- Check resistor value (should be 220Ω)
- Verify power supply voltage
- Check for loose connections

**Issue**: LED flickering
**Solution**:
- Check for loose connections
- Verify stable power supply
- Check code for rapid state changes

### Web Server Issues

**Issue**: Can't access web server
**Solution**:
- Check IP address in Serial Monitor
- Ensure same network as computer
- Try different browser
- Check firewall settings
- Verify server started successfully

**Issue**: API endpoints not responding
**Solution**:
- Check endpoint URLs
- Verify HTTP method (GET/POST)
- Check Serial Monitor for errors
- Test with simple endpoints first

### OTA Update Problems

**Issue**: OTA update fails
**Solution**:
- Check password (`workshop123`)
- Verify network connection
- Ensure sufficient free memory
- Try restarting ESP8266
- Check OTA library version

**Issue**: "Authentication failed"
**Solution**:
- Verify OTA password in code
- Check password in `platformio.ini`
- Ensure password matches exactly

### Hardware Issues

**Issue**: ESP8266 not responding
**Solution**:
- Check power supply
- Verify USB connection
- Try different board
- Check for physical damage
- Reset board (hold RESET button)

**Issue**: OLED display not working
**Solution**:
- Check I2C connections (SDA, SCL)
- Verify display address
- Check power connections
- Try different display
- Verify library installation

### Code Compilation Issues

**Issue**: Compilation errors
**Solution**:
- Check library dependencies
- Verify `platformio.ini` configuration
- Update PlatformIO: `pio upgrade`
- Check for syntax errors
- Verify include statements

**Issue**: "Library not found"
**Solution**:
- Check `lib_deps` in `platformio.ini`
- Run `pio lib install [library_name]`
- Verify library names are correct
- Check internet connection

### Network Testing

**Issue**: Can't ping ESP8266
**Solution**:
- Check IP address
- Verify same subnet
- Check router settings
- Try different device
- Check network isolation

**Issue**: ngrok not working
**Solution**:
- Check ngrok installation
- Verify account setup
- Try different port
- Check firewall settings
- Use local network only as backup

## Debugging Tips

### Serial Monitor Debugging
```cpp
void setup() {
  Serial.begin(115200);
  Serial.println("Starting debug...");
  
  // Add debug prints
  Serial.print("WiFi Status: ");
  Serial.println(WiFi.status());
  
  Serial.print("Free Heap: ");
  Serial.println(ESP.getFreeHeap());
}
```

### LED Testing
```cpp
void testLEDs() {
  // Test red LED (D2)
  pinMode(D2, OUTPUT);
  digitalWrite(D2, HIGH);
  delay(1000);
  digitalWrite(D2, LOW);
  
  // Test green LED (D3)
  pinMode(D3, OUTPUT);
  digitalWrite(D3, HIGH);
  delay(1000);
  digitalWrite(D3, LOW);
}
```

### Network Testing
```cpp
void testNetwork() {
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  
  Serial.print("MAC Address: ");
  Serial.println(WiFi.macAddress());
  
  Serial.print("Signal Strength: ");
  Serial.println(WiFi.RSSI());
}
```

## Emergency Procedures

### Board Reset
1. Hold RESET button for 5 seconds
2. Power cycle the board
3. Try upload again

### Factory Reset
1. Hold RESET + BOOT buttons
2. Power on board
3. Release RESET, then BOOT
4. Upload basic blink code

### Network Reset
1. Change WiFi credentials
2. Restart ESP8266
3. Check Serial Monitor for new IP

## Getting Help

1. Check Serial Monitor output first
2. Verify all connections
3. Test with simple examples
4. Ask instructor for help
5. Use multimeter to test connections
6. Try backup hardware if available

## Prevention Tips

- Always check connections before powering on. Ask me to check the wires on first times until you get the hang of it.
- Use resistors provided
- Verify LED polarity
- Test code incrementally
- Keep backup components ready
- Document your setup
