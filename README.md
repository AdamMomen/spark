# Spark IoT Workshop

ESP8266-based IoT workshop materials for hands-on learning.

## Workshop Overview

**Date**: October 19, 2025  
**Venue**: Network School, Johor, Malaysia  
**Duration**: 4 hours  
**Teams**: 4 teams of 2 participants each  
**Hardware**: ESP8266MOD + OLED display boards

## Workshop Schedule

1. **Setup & Circuit Building** (1 hour) 12:00 - 13:00
   - PlatformIO installation and configuration
   - Breadboard basics and LED circuit
   - Serial monitor testing
2. Lunch Break (30 minutes) 13:00 - 13:30

3. **Web Server & API** (1 hour) -13:30 -> 14:30
   - REST API development
   - LED control via HTTP
   - Local network testing

4. **Dashboard Development** (1.5 minutes)
   - HTML/CSS/JS dashboard plugin
   - Mobile-responsive design

5. **Demos & Presentations** (0.5 hour)
   - MCP integration showcase
   - ngrok tunnel demonstrations

5. **Challenge & Presentations** (1 hour)
   - Challenge: Most creative IoT dashboard
   - Team presentations

## Quick Start

1. Follow [setup.md](docs/setup.md) for PlatformIO installation
2. Read [lect1.md](docs/lect1.md) for LED basics
3. Check [api-spec.md](docs/api-spec.md) for API documentation
4. Use examples in `/examples` directory

## Hardware Requirements

- ESP8266MOD board with OLED display
- LEDs (Red, Green)
- Resistors (220Ω)
- Breadboard and jumper wires
- USB cable

## Files Structure

```
spark-workshop/
├── docs/                    # Workshop documentation
├── src/                     # Main source code
├── examples/                # Code examples
├── web/                     # Dashboard templates
└── platformio.ini          # PlatformIO configuration
```

## Support

For troubleshooting, see [troubleshooting.md](docs/troubleshooting.md)
