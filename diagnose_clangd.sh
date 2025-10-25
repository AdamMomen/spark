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
