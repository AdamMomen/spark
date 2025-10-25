#!/bin/bash

# Disable Clangd and Use PlatformIO IntelliSense Instead
# This is often more reliable for ESP8266 projects

echo "🔧 Switching from Clangd to PlatformIO IntelliSense"
echo "=================================================="

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

print_status() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

# Create VS Code settings that disable clangd and enable PlatformIO IntelliSense
print_status "Creating VS Code/Cursor settings for PlatformIO IntelliSense..."

mkdir -p .vscode
cat > .vscode/settings.json << 'EOF'
{
    "C_Cpp.intelliSenseEngine": "default",
    "C_Cpp.errorSquiggles": "enabled",
    "C_Cpp.intelliSenseEngine": "Tag Parser",
    "C_Cpp.autocomplete": "default",
    "C_Cpp.suggestSnippets": true,
    "C_Cpp.enhancedColorization": "enabled",
    "C_Cpp.vcpkg.enabled": false,
    "C_Cpp.default.compilerPath": "",
    "C_Cpp.default.cStandard": "c11",
    "C_Cpp.default.cppStandard": "c++17",
    "C_Cpp.default.includePath": [
        "${workspaceFolder}/src",
        "${workspaceFolder}/include",
        "${workspaceFolder}/lib",
        "${workspaceFolder}/.pio/libdeps/nodemcuv2/Adafruit SSD1306/src",
        "${workspaceFolder}/.pio/libdeps/nodemcuv2/Adafruit GFX Library",
        "${workspaceFolder}/.pio/libdeps/nodemcuv2/ESP8266WiFi/src",
        "${workspaceFolder}/.pio/libdeps/nodemcuv2/ESP8266WebServer/src",
        "${workspaceFolder}/.pio/libdeps/nodemcuv2/ArduinoOTA/src",
        "${workspaceFolder}/.pio/libdeps/nodemcuv2/Wire/src",
        "${workspaceFolder}/.pio/libdeps/nodemcuv2/SPI/src",
        "${workspaceFolder}/.pio/libdeps/nodemcuv2/WebSockets/src",
        "${workspaceFolder}/.platformio/packages/framework-arduinoespressif8266/cores/esp8266",
        "${workspaceFolder}/.platformio/packages/framework-arduinoespressif8266/variants/generic",
        "${workspaceFolder}/.platformio/packages/framework-arduinoespressif8266/libraries/ESP8266WiFi/src",
        "${workspaceFolder}/.platformio/packages/framework-arduinoespressif8266/libraries/ESP8266WebServer/src",
        "${workspaceFolder}/.platformio/packages/framework-arduinoespressif8266/libraries/ArduinoOTA/src",
        "${workspaceFolder}/.platformio/packages/framework-arduinoespressif8266/libraries/Wire/src",
        "${workspaceFolder}/.platformio/packages/framework-arduinoespressif8266/libraries/SPI/src"
    ],
    "C_Cpp.default.defines": [
        "ARDUINO_ARCH_ESP8266",
        "ESP8266",
        "ARDUINO=10805",
        "F_CPU=80000000L",
        "PLATFORMIO=60118",
        "__ets__",
        "ICACHE_FLASH",
        "SSIZE_MAX",
        "LWIP_NO_UNISTD_H=1",
        "ssize_t=long",
        "_SSIZE_T_DECLARED",
        "ARDUINO_BOARD=\"ESP8266\"",
        "ARDUINO_VARIANT=\"generic\""
    ],
    "C_Cpp.default.browse.limitSymbolsToIncludedHeaders": true,
    "C_Cpp.default.browse.databaseFilename": "${workspaceFolder}/.vscode/browse.vc.db",
    "C_Cpp.default.browse.path": [
        "${workspaceFolder}/src",
        "${workspaceFolder}/include",
        "${workspaceFolder}/lib",
        "${workspaceFolder}/.pio/libdeps/nodemcuv2",
        "${workspaceFolder}/.platformio/packages/framework-arduinoespressif8266"
    ],
    "files.associations": {
        "*.h": "cpp",
        "*.hpp": "cpp",
        "*.cpp": "cpp",
        "*.ino": "cpp"
    },
    "clangd.enabled": false,
    "clangd.arguments": [],
    "clangd.fallbackFlags": []
}
EOF

print_success "VS Code/Cursor settings updated ✓"

# Remove .clangd file to prevent conflicts
if [ -f ".clangd" ]; then
    print_status "Removing .clangd file to prevent conflicts..."
    mv .clangd .clangd.backup
    print_success ".clangd file backed up ✓"
fi

# Generate PlatformIO IntelliSense configuration
print_status "Generating PlatformIO IntelliSense configuration..."
if pio run -t compiledb; then
    print_success "PlatformIO IntelliSense configuration generated ✓"
else
    print_error "Failed to generate PlatformIO IntelliSense configuration"
    exit 1
fi

# Create a script to switch back to clangd if needed
cat > switch_to_clangd.sh << 'EOF'
#!/bin/bash
echo "🔄 Switching back to Clangd..."
mv .clangd.backup .clangd 2>/dev/null || echo "No .clangd backup found"
echo "✅ Clangd configuration restored"
echo "Please restart your editor to apply changes"
EOF

chmod +x switch_to_clangd.sh
print_success "Switch-back script created ✓"

# Final recommendations
echo ""
echo "🎉 Switched to PlatformIO IntelliSense!"
echo "======================================"
echo ""
print_success "What was done:"
echo "  ✅ Disabled clangd"
echo "  ✅ Enabled PlatformIO IntelliSense"
echo "  ✅ Configured proper include paths"
echo "  ✅ Added ESP8266 definitions"
echo "  ✅ Created switch-back script"
echo ""
print_status "Next steps:"
echo "  1. Restart your editor (Cmd+Shift+P → 'Developer: Reload Window')"
echo "  2. Wait 30 seconds for IntelliSense to reindex"
echo "  3. You should now have proper autocomplete and error detection"
echo ""
print_warning "If you want to switch back to clangd later, run: ./switch_to_clangd.sh"
echo ""
print_success "PlatformIO IntelliSense is often more reliable for ESP8266 projects!"


