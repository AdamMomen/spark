#!/bin/bash
# Script to restart C/C++ extension and refresh IntelliSense

echo "🔄 Restarting C/C++ Extension for PlatformIO..."
echo "=============================================="

# Kill any existing C/C++ extension processes
echo "Stopping C/C++ extension processes..."
pkill -f "C/C++" || true
pkill -f "cpptools" || true

# Wait a moment
sleep 2

echo ""
echo "✅ C/C++ Extension restarted!"
echo ""
echo "📋 Next steps:"
echo "   1. Reload your editor window: Cmd+Shift+P → 'Developer: Reload Window'"
echo "   2. Wait 30-60 seconds for IntelliSense to rebuild the database"
echo "   3. Try 'Go to Definition' (Cmd+Click or F12) on any function"
echo ""
echo "🔍 To verify setup:"
echo "   - Open src/main.cpp"
echo "   - Try to go to definition of 'setup()' or 'loop()'"
echo "   - Try to go to definition of WorkshopESP methods"
echo ""
echo "⚠️  If go-to-definition still doesn't work:"
echo "   - Check the Output panel → 'C/C++' for any errors"
echo "   - Try Cmd+Shift+P → 'C/C++: Reset IntelliSense Database'"
echo "   - Make sure the C/C++ extension is enabled and not conflicting with other extensions"
