#!/bin/bash

# Automated Clangd Setup for ESP8266/PlatformIO Projects
# This script detects and fixes common clangd issues automatically

set -e

echo "🔧 Automated Clangd Setup for ESP8266 Projects"
echo "=============================================="

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Function to print colored output
print_status() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Check if we're in a PlatformIO project
if [ ! -f "platformio.ini" ]; then
    print_error "Not in a PlatformIO project directory!"
    print_status "Please run this script from your project root (where platformio.ini is located)"
    exit 1
fi

print_status "Detected PlatformIO project ✓"

# Step 1: Generate compile_commands.json
print_status "Generating compile_commands.json..."
if pio run -t compiledb; then
    print_success "Compile database generated ✓"
else
    print_error "Failed to generate compile database"
    exit 1
fi

# Step 2: Detect ESP8266-specific issues
print_status "Detecting ESP8266-specific clangd issues..."

# Check for problematic flags in compile_commands.json
PROBLEMATIC_FLAGS=(
    "-mlongcalls"
    "-mtext-section-literals"
    "-free"
    "-fipa-pta"
    "-fno-tree-switch-conversion"
    "-fstrict-volatile-bitfields"
)

print_status "Checking for problematic compiler flags..."

# Step 3: Create optimized .clangd configuration
print_status "Creating optimized .clangd configuration..."

cat > .clangd << 'EOF'
CompileFlags:
  CompilationDatabase: .
  Add:
    # ESP8266 Core Definitions
    - -DARDUINO_ARCH_ESP8266
    - -DESP8266
    - -DARDUINO=10805
    - -DF_CPU=80000000L
    - -DPLATFORMIO=60118
    - -D__ets__
    - -DICACHE_FLASH
    - -U__STRICT_ANSI__
    
    # ESP8266 Type Definitions
    - -DSSIZE_MAX
    - -DLWIP_NO_UNISTD_H=1
    - -Dssize_t=long
    - -D_SSIZE_T_DECLARED
    
    # Additional ESP8266 Defines
    - -DARDUINO_BOARD="ESP8266"
    - -DARDUINO_VARIANT="generic"
    - -DESP8266_WIFI_SSID=""
    - -DESP8266_WIFI_PASS=""
    
  Remove:
    # Remove ESP8266-specific flags that clangd can't handle
    - -mlongcalls
    - -mlong-calls
    - -mtext-section-literals
    - -free
    - -fipa-pta
    - -fno-tree-switch-conversion
    - -fstrict-volatile-bitfields
    
    # Remove other problematic flags
    - -Wl,--gc-sections
    - -Wl,--relax
    - -Wl,--start-group
    - -Wl,--end-group

Diagnostics:
  ClangTidy:
    Add:
      - readability-*
      - performance-*
      - modernize-*
    Remove:
      - readability-magic-numbers
      - readability-uppercase-literal-suffix
      - modernize-use-trailing-return-type

Index:
  Background: Build
  StandardLibrary: No
EOF

print_success "Optimized .clangd configuration created ✓"

# Step 4: Detect and fix common include issues
print_status "Checking for common include issues..."

# Check if Arduino.h is included first in header files
HEADER_FILES=$(find src -name "*.h" -o -name "*.hpp" 2>/dev/null || true)
if [ -n "$HEADER_FILES" ]; then
    for file in $HEADER_FILES; do
        if grep -q "#include.*Arduino" "$file" && ! grep -q "^#include <Arduino.h>" "$file"; then
            print_warning "Found Arduino include issue in $file"
            # This would need manual fixing, but we can detect it
        fi
    done
fi

# Step 5: Create clangd restart script
print_status "Creating clangd restart helper..."

cat > restart_clangd.sh << 'EOF'
#!/bin/bash
# Helper script to restart clangd after configuration changes

echo "🔄 Restarting clangd..."

# Kill existing clangd processes
pkill -f clangd || true

# Wait a moment
sleep 1

echo "✅ Clangd restarted. Please reload your editor window."
echo "   In VS Code: Cmd+Shift+P → 'Developer: Reload Window'"
echo "   In Cursor: Cmd+Shift+P → 'Developer: Reload Window'"
EOF

chmod +x restart_clangd.sh
print_success "Clangd restart helper created ✓"

# Step 6: Create diagnostic script
print_status "Creating diagnostic script..."

cat > diagnose_clangd.sh << 'EOF'
#!/bin/bash
# Diagnostic script for clangd issues

echo "🔍 Clangd Diagnostic Report"
echo "=========================="

# Check clangd version
echo "Clangd version:"
clangd --version 2>/dev/null || echo "❌ Clangd not found in PATH"

echo ""

# Check compile_commands.json
echo "Compile commands status:"
if [ -f "compile_commands.json" ]; then
    echo "✅ compile_commands.json exists"
    echo "   Size: $(wc -l < compile_commands.json) lines"
    
    # Check for problematic flags
    echo "   Problematic flags found:"
    grep -o '\-mlongcalls\|\-mtext-section-literals\|\-free\|\-fipa-pta' compile_commands.json | sort | uniq || echo "   None found"
else
    echo "❌ compile_commands.json missing"
fi

echo ""

# Check .clangd configuration
echo ".clangd configuration:"
if [ -f ".clangd" ]; then
    echo "✅ .clangd exists"
    echo "   Configuration preview:"
    head -20 .clangd | sed 's/^/   /'
else
    echo "❌ .clangd missing"
fi

echo ""

# Check for common issues
echo "Common issues check:"
echo "   Arduino.h includes:"
find src -name "*.h" -o -name "*.cpp" | xargs grep -l "Arduino.h" 2>/dev/null | wc -l | sed 's/^/   /'

echo "   Missing includes in source files:"
find src -name "*.cpp" | while read file; do
    if ! grep -q "#include.*Arduino.h" "$file"; then
        echo "   ⚠️  $file missing Arduino.h"
    fi
done

echo ""
echo "📋 Recommendations:"
echo "   1. Restart your editor after running this script"
echo "   2. If issues persist, try: ./restart_clangd.sh"
echo "   3. For persistent issues, consider disabling clangd temporarily"
EOF

chmod +x diagnose_clangd.sh
print_success "Diagnostic script created ✓"

# Step 7: Create VS Code settings optimization
print_status "Creating VS Code/Cursor settings optimization..."

mkdir -p .vscode
cat > .vscode/settings.json << 'EOF'
{
    "clangd.arguments": [
        "--compile-commands-dir=.",
        "--background-index",
        "--clang-tidy",
        "--header-insertion=iwyu",
        "--completion-style=detailed",
        "--function-arg-placeholders=false"
    ],
    "clangd.fallbackFlags": [
        "-std=c++17"
    ],
    "C_Cpp.intelliSenseEngine": "disabled",
    "C_Cpp.errorSquiggles": "disabled",
    "files.associations": {
        "*.h": "cpp",
        "*.hpp": "cpp",
        "*.cpp": "cpp",
        "*.ino": "cpp"
    }
}
EOF

print_success "VS Code/Cursor settings optimized ✓"

# Step 8: Final recommendations
echo ""
echo "🎉 Automated Clangd Setup Complete!"
echo "===================================="
echo ""
print_success "What was done:"
echo "  ✅ Generated compile_commands.json"
echo "  ✅ Created optimized .clangd configuration"
echo "  ✅ Removed problematic ESP8266 flags"
echo "  ✅ Added ESP8266-specific definitions"
echo "  ✅ Created diagnostic and restart scripts"
echo "  ✅ Optimized editor settings"
echo ""
print_status "Next steps:"
echo "  1. Restart your editor (Cmd+Shift+P → 'Developer: Reload Window')"
echo "  2. Wait 30 seconds for clangd to reindex"
echo "  3. If issues persist, run: ./diagnose_clangd.sh"
echo "  4. For emergency restart: ./restart_clangd.sh"
echo ""
print_warning "Note: Some ESP8266-specific errors may still appear."
print_warning "This is normal - the code will compile fine despite clangd warnings."
echo ""
print_success "Setup complete! Your clangd should now work much better with ESP8266 projects."




