#!/bin/bash
# Comprehensive troubleshooting script for C/C++ go-to-definition issues

echo "🔍 C/C++ Go-to-Definition Troubleshooting"
echo "=========================================="

# Check if we're in a PlatformIO project
if [ ! -f "platformio.ini" ]; then
    echo "❌ Not in a PlatformIO project directory!"
    exit 1
fi

echo "✅ PlatformIO project detected"

# Check compile_commands.json
echo ""
echo "📋 Checking compile_commands.json..."
if [ -f "compile_commands.json" ]; then
    echo "✅ compile_commands.json exists ($(wc -l < compile_commands.json) lines)"
else
    echo "❌ compile_commands.json missing - generating..."
    pio run -t compiledb
fi

# Check .vscode configuration
echo ""
echo "📋 Checking .vscode configuration..."
if [ -f ".vscode/settings.json" ]; then
    echo "✅ .vscode/settings.json exists"
else
    echo "❌ .vscode/settings.json missing"
fi

if [ -f ".vscode/c_cpp_properties.json" ]; then
    echo "✅ .vscode/c_cpp_properties.json exists"
else
    echo "❌ .vscode/c_cpp_properties.json missing"
fi

# Check for browse database
echo ""
echo "📋 Checking IntelliSense database..."
if [ -f ".vscode/browse.vc.db" ]; then
    echo "✅ IntelliSense database exists ($(du -h .vscode/browse.vc.db | cut -f1))"
else
    echo "⚠️  IntelliSense database not found (will be created on first use)"
fi

echo ""
echo "🔧 SOLUTION: Install the correct extension!"
echo "=========================================="
echo ""
echo "The issue is likely that you're using the 'C/C++ Runner' extension"
echo "instead of the 'Microsoft C/C++' extension."
echo ""
echo "📥 Required Extensions:"
echo "   1. Microsoft C/C++ (ms-vscode.cpptools)"
echo "   2. PlatformIO IDE (platformio.platformio-ide)"
echo ""
echo "❌ Remove these extensions (they don't provide go-to-definition):"
echo "   - C/C++ Runner"
echo "   - Code Runner"
echo ""
echo "🔄 Steps to fix:"
echo "   1. Uninstall 'C/C++ Runner' extension"
echo "   2. Install 'Microsoft C/C++' extension"
echo "   3. Reload window: Cmd+Shift+P → 'Developer: Reload Window'"
echo "   4. Wait 60 seconds for IntelliSense to build database"
echo "   5. Try go-to-definition: Cmd+Click or F12"
echo ""
echo "🧪 Test go-to-definition:"
echo "   - Open src/main.cpp"
echo "   - Try Cmd+Click on 'setupWiFi' or 'handleClient'"
echo "   - Try F12 on any function name"
echo ""
echo "📊 Check IntelliSense status:"
echo "   - Look at bottom status bar for 'C/C++'"
echo "   - Check Output panel → 'C/C++' for errors"
echo "   - Try Cmd+Shift+P → 'C/C++: Reset IntelliSense Database'"







