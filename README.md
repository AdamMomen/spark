# Spark IoT Workshop

ESP8266-based IoT workshop materials for hands-on learning.

## Workshop Overview

**Date**: October 26, 2025  
**Duration**: 4 hours  

**Hardware**: ESP8266

## Workshop Schedule

1. **[Setup](./docs/setup.md)** (90 minutes)
   - PlatformIO installation and configuration
   - Breadboard basics and LED circuit
   - Serial monitor testing

2. Break (15 minutes)

3. **[Exercise 1: Web Server Control](./docs/exercise1.md)** (30 minutes)
   - REST API development
   - LED control via HTTP
   - Local network testing

3. **[Exercise 2: Potentiometer](./docs/exercise2.md)** (90 minutes)
   - Read Potentiometer inputs
   - Websockets for real time updates

## Hardware Requirements

- ESP8266MOD board with OLED display
- LEDs (Red, Green)
- Resistors (320Ω)
- Potentiometer (1kΩ)
- M-M Connection Wires
- Breadboard and jumper wires
- USB cable

## Files Structure

```
spark/
├── docs/                    # Workshop documentation
├── src/                     # Main source code
├── examples/                # Code examples
└── platformio.ini          # PlatformIO configuration
```

## Support

For troubleshooting, see [troubleshooting.md](docs/troubleshooting.md)
