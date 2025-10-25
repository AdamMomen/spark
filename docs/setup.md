# Workshop Setup Instructions

## Prerequisites

- **macOS**, **Linux**, or **Windows** (for this workshop)
- Python 3.7+ installed
- Cursor IDE installed

## Quick Setup (Recommended)

### macOS & Linux
```bash
bash setup.sh
```

### Windows (PowerShell)
```powershell
powershell setup.ps1
```

## Manual Setup 

## Step 1: Install PlatformIO

### Method 1: Using pip (Recommended)
```bash
pip install platformio
```

### Method 2: Using pipx (Alternative)
```bash
pipx install platformio
```

### Verify Installation
```bash
pio --version
```

## Step 2: Install Cursor Extensions

1. Open Cursor IDE
2. Go to Extensions (Cmd+Shift+X)
3. Install these extensions:
   - **clangd**: `llvm-vs-code-extensions.vscode-clangd`


## Step 3: Install ESP8266 Drivers (if needed)

### For macOS:
- Most ESP8266 boards work with built-in drivers
- If you get "device not found" errors, install CH340 drivers:
  ```bash
  brew install --cask wch-ch34x-usb-serial-driver
  ```

### For Windows:
- Install CH340 drivers: `choco install ch340-driver -y`
- Or install CP2102 drivers: `choco install cp2102-driver -y`
- Or download from manufacturer website

## Step 4: Test Your Setup

1. Clone this repository
2. Open terminal in project directory
3. Run: `pio run`
4. Connect your ESP8266 board via USB
5. Run: `pio run --target upload`

## Troubleshooting

### Serial Monitor Issues
- **Baud Rate**: Always use 115200
- **Port (macOS)**: Check `/dev/tty.usbserial-*` or `/dev/tty.SLAB_USBtoUART`
- **Port (Windows)**: Check `COM3`, `COM4`, etc.
- **Permission (macOS)**: Run `sudo chmod 666 /dev/tty.usbserial-*`

### Upload Issues
- **Hold Reset Button**: Press and hold RESET button, then press and hold BOOT button, release RESET, then release BOOT
- **Wrong Port**: Check `pio device list` to find correct port
- **Driver Issues**: 
  - **macOS**: Reinstall CH340 drivers
  - **Windows**: Install CH340 or CP2102 drivers

### WiFi Issues
- **SSID**: Update `WIFI_SSID` in `platformio.ini`
- **Password**: Update `WIFI_PASSWORD` in `platformio.ini`
- **Network**: Ensure 2.4GHz network (ESP8266 doesn't support 5GHz)

## Next Steps

1. Read [exercise1.md](exercise1.md) for LED basics
2. Start with examples in `/examples` directory
