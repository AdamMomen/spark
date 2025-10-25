# Spark IoT Workshop

ESP8266-based IoT workshop materials for hands-on learning.

## Workshop Overview

**Date**: October 26, 2025  
**Duration**: 4 hours  

**Hardware**: ESP8266

## Workshop Schedule

1. **Setup** (90 minutes)
   - PlatformIO installation and configuration
   - Breadboard basics and LED circuit
   - Serial monitor testing

2. 15 minutes Break

3. **[Exercise 1: Web Server Control](./docs/exercise1.md)** (30 hour)
   - REST API development
   - LED control via HTTP
   - Local network testing

3. **[Exercise 2: Potentiometer Exercise](./docs/exercise2.md)** (1 hour)
   - Read Potentiometer inputs
   - 

## Quick Start

1. Follow [setup.md](docs/setup.md) for setup tools and IDEs on your machine
2. Read [exercise1.md](docs/exercise1.md) for LED & Digit Write
3. Read [exercise2.md](docs/exercise2.md) for Potentiometer & Analog Read
4. Use examples in `/examples` directory

## Hardware Requirements

- ESP8266MOD board with OLED display
- LEDs (Red, Green)
- Resistors (320Ω)
- Potentiometer (1kΩ)
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
