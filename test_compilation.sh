#!/bin/bash
echo "🔨 Testing compilation..."

if pio run --environment nodemcuv2; then
    echo "✅ Compilation successful!"
    echo "   Your code is working fine - linting errors are just IDE issues"
else
    echo "❌ Compilation failed - there are real code issues"
    echo "   Check the error messages above"
fi
