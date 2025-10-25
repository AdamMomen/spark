#!/bin/bash

echo "Restarting clangd for ESP8266 development..."

# Kill any existing clangd processes
pkill clangd

# Wait a moment for processes to fully terminate
sleep 1

# Regenerate compile_commands.json
echo "Regenerating compile_commands.json..."
pio run --target compiledb

echo "clangd should restart automatically in your IDE."
echo "If not, try reloading your IDE window."