#!/bin/bash
echo "🔧 Re-enabling linting..."

# Create basic PlatformIO IntelliSense settings
mkdir -p .vscode
cat > .vscode/settings.json << 'EOL'
{
    "C_Cpp.intelliSenseEngine": "default",
    "C_Cpp.errorSquiggles": "enabled",
    "C_Cpp.autocomplete": "default",
    "C_Cpp.suggestSnippets": true,
    "C_Cpp.enhancedColorization": "enabled",
    "clangd.enabled": false,
    "files.associations": {
        "*.h": "cpp",
        "*.hpp": "cpp",
        "*.cpp": "cpp",
        "*.ino": "cpp"
    }
}
EOL

# Generate compile database
pio run -t compiledb

echo "✅ Linting re-enabled with PlatformIO IntelliSense"
echo "Please restart your editor"
