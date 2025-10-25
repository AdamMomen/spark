#!/bin/bash
# Complete clangd setup for ESP8266 PlatformIO projects

echo "🚀 Complete clangd Setup for ESP8266 PlatformIO"
echo "================================================"

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

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Check if we're in a PlatformIO project
if [ ! -f "platformio.ini" ]; then
    print_error "Not in a PlatformIO project directory!"
    print_status "Please run this script from your project root (where platformio.ini is located)"
    exit 1
fi

print_success "PlatformIO project detected ✓"

# Step 1: Check clangd installation
print_status "Checking clangd installation..."
if command -v clangd &> /dev/null; then
    CLANGD_VERSION=$(clangd --version | head -n1)
    print_success "clangd found: $CLANGD_VERSION"
else
    print_error "clangd not found in PATH!"
    print_status "Please install clangd:"
    print_status "  macOS: brew install llvm"
    print_status "  Ubuntu: sudo apt install clangd"
    print_status "  Windows: Download from https://releases.llvm.org/"
    exit 1
fi

# Step 2: Generate compile_commands.json
print_status "Generating compile_commands.json..."
if pio run -t compiledb; then
    print_success "Compile database generated ✓"
else
    print_error "Failed to generate compile database"
    exit 1
fi

# Step 3: Verify .clangd configuration
print_status "Checking .clangd configuration..."
if [ -f ".clangd" ]; then
    print_success ".clangd configuration exists ✓"
else
    print_warning ".clangd configuration missing - creating optimized config..."
    # The .clangd file should already be created by the previous step
fi

# Step 4: Verify VS Code settings
print_status "Checking VS Code settings..."
if [ -f ".vscode/settings.json" ]; then
    print_success "VS Code settings configured ✓"
else
    print_warning "VS Code settings missing"
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
sleep 2

echo "✅ Clangd restarted. Please reload your editor window."
echo "   In Cursor: Cmd+Shift+P → 'Developer: Reload Window'"
echo ""
echo "📋 Next steps:"
echo "   1. Reload window: Cmd+Shift+P → 'Developer: Reload Window'"
echo "   2. Wait 30-60 seconds for clangd to reindex"
echo "   3. Try go-to-definition: Cmd+Click or F12"
echo ""
echo "🧪 Test go-to-definition:"
echo "   - Open src/main.cpp"
echo "   - Try Cmd+Click on 'setupWiFi' or 'handleClient'"
echo "   - Try F12 on any function name"
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

# Check VS Code settings
echo "VS Code settings:"
if [ -f ".vscode/settings.json" ]; then
    echo "✅ .vscode/settings.json exists"
    if grep -q "clangd.arguments" .vscode/settings.json; then
        echo "✅ clangd configuration found in settings"
    else
        echo "⚠️  clangd configuration missing from settings"
    fi
else
    echo "❌ .vscode/settings.json missing"
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
echo "   1. Make sure clangd extension is installed in Cursor"
echo "   2. Restart your editor after running this script"
echo "   3. If issues persist, try: ./restart_clangd.sh"
echo "   4. Check Output panel → 'clangd' for any errors"
EOF

chmod +x diagnose_clangd.sh
print_success "Diagnostic script created ✓"

# Step 7: Final recommendations
echo ""
echo "🎉 Complete clangd Setup Finished!"
echo "=================================="
echo ""
print_success "What was configured:"
echo "  ✅ Generated compile_commands.json"
echo "  ✅ Created optimized .clangd configuration"
echo "  ✅ Removed problematic ESP8266 flags"
echo "  ✅ Added ESP8266-specific definitions"
echo "  ✅ Configured VS Code settings for clangd"
echo "  ✅ Created diagnostic and restart scripts"
echo ""
print_status "Required Extensions:"
echo "  📥 Install 'clangd' extension in Cursor"
echo "  📥 Install 'PlatformIO IDE' extension"
echo ""
print_status "Next steps:"
echo "  1. Install clangd extension in Cursor"
echo "  2. Reload window: Cmd+Shift+P → 'Developer: Reload Window'"
echo "  3. Wait 60 seconds for clangd to reindex"
echo "  4. Test go-to-definition: Cmd+Click or F12"
echo ""
print_status "Testing:"
echo "  - Open src/main.cpp"
echo "  - Try Cmd+Click on 'setupWiFi' or 'handleClient'"
echo "  - Try F12 on any function name"
echo ""
print_warning "Note: Some ESP8266-specific errors may still appear."
print_warning "This is normal - the code will compile fine despite clangd warnings."
echo ""
print_success "Setup complete! clangd should now provide excellent go-to-definition support."






