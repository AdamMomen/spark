# IoT Workshop Setup Script for Windows
# Automates the setup process from setup.md

param(
  [switch]$Force
)

# Set error action preference
$ErrorActionPreference = "Stop"

Write-Host "🚀 IoT Workshop Setup Script (Windows)" -ForegroundColor Green
Write-Host "=====================================" -ForegroundColor Green

# Function to print colored output
function Write-Status {
  param([string]$Message)
  Write-Host "✅ $Message" -ForegroundColor Green
}

function Write-Warning {
  param([string]$Message)
  Write-Host "⚠️  $Message" -ForegroundColor Yellow
}

function Write-Error {
  param([string]$Message)
  Write-Host "❌ $Message" -ForegroundColor Red
}

function Write-Info {
  param([string]$Message)
  Write-Host "ℹ️  $Message" -ForegroundColor Blue
}

# Step 1: Check if running on Windows
Write-Host ""
Write-Host "📋 Step 1: Checking Windows system..." -ForegroundColor Cyan
if ($env:OS -ne "Windows_NT") {
  Write-Error "This script is designed for Windows only"
  exit 1
}
Write-Status "Detected Windows system"

# Step 2: Check and install Chocolatey
Write-Host ""
Write-Host "📋 Step 2: Checking Chocolatey package manager..." -ForegroundColor Cyan
try {
  $chocoVersion = choco --version
  Write-Status "Chocolatey $chocoVersion found"
}
catch {
  Write-Warning "Chocolatey not found. Installing Chocolatey..."
  try {
    Set-ExecutionPolicy Bypass -Scope Process -Force
    [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072
    iex ((New-Object System.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1'))
    Write-Status "Chocolatey installed successfully"
  }
  catch {
    Write-Error "Failed to install Chocolatey. Please install manually from https://chocolatey.org/install"
    exit 1
  }
}

# Step 3: Check and install Python
Write-Host ""
Write-Host "📋 Step 3: Checking Python installation..." -ForegroundColor Cyan
try {
  $pythonVersion = python --version
  Write-Status "Python $pythonVersion found"
}
catch {
  Write-Warning "Python not found. Installing Python via Chocolatey..."
  try {
    choco install python -y
    Write-Status "Python installed successfully"
        
    # Refresh environment variables
    $env:Path = [System.Environment]::GetEnvironmentVariable("Path", "Machine") + ";" + [System.Environment]::GetEnvironmentVariable("Path", "User")
        
    # Verify installation
    $pythonVersion = python --version
    Write-Status "Python $pythonVersion now available"
  }
  catch {
    Write-Error "Failed to install Python. Please install manually from https://python.org"
    exit 1
  }
}

# Step 4: Install PlatformIO
Write-Host ""
Write-Host "📋 Step 4: Installing PlatformIO..." -ForegroundColor Cyan
try {
  $pioVersion = pio --version
  Write-Status "PlatformIO already installed: $pioVersion"
}
catch {
  Write-Warning "Installing PlatformIO via pip..."
  try {
    pip install platformio
    Write-Status "PlatformIO installed successfully"
  }
  catch {
    Write-Error "Failed to install PlatformIO. Please install manually: pip install platformio"
    exit 1
  }
}

# Step 5: Install ESP8266 drivers
Write-Host ""
Write-Host "📋 Step 5: Checking ESP8266 drivers..." -ForegroundColor Cyan
Write-Warning "For ESP8266 devices, you may need to install drivers:"
Write-Host "  - CH340 drivers: choco install ch340-driver -y"
Write-Host "  - CP2102 drivers: choco install cp2102-driver -y"
Write-Host "  - Or download from manufacturer website"

# Step 6: Setup clangd configuration
Write-Host ""
Write-Host "📋 Step 6: Setting up clangd configuration..." -ForegroundColor Cyan
if (Test-Path ".clang_template") {
  Write-Status "Found .clang_template"
  # Generate compile_commands.json for better clangd support
  Write-Warning "Generating compile_commands.json..."
  try {
    pio run --target compiledb 2>$null
    Write-Status "clangd configuration ready"
  }
  catch {
    Write-Warning "CompileDB generation failed (this is optional)"
  }
}
else {
  Write-Warning ".clang_template not found, using default clangd config"
}

# Step 7: Test compilation
Write-Host ""
Write-Host "📋 Step 7: Testing compilation..." -ForegroundColor Cyan
try {
  pio run --target checkprogsize 2>$null
  Write-Status "Compilation test passed"
}
catch {
  Write-Warning "Compilation test failed - this is normal if no code is uploaded yet"
}

# Step 8: Check for ESP8266 device and update platformio.ini
Write-Host ""
Write-Host "📋 Step 8: Checking for ESP8266 devices..." -ForegroundColor Cyan
try {
  $devices = pio device list
  if ($devices -match "USB|COM") {
    Write-Status "ESP8266 device detected"
    Write-Host "Available devices:"
    Write-Host $devices
    
    # Find the first USB/COM device and update platformio.ini
    $usbDevice = ($devices | Select-String "USB|COM" | Select-Object -First 1).Line.Split()[0]
    if ($usbDevice) {
      Write-Info "Updating platformio.ini with device: $usbDevice"
      
      # Update platformio.ini with the detected USB device
      $platformioContent = Get-Content platformio.ini
      $updatedContent = $platformioContent -replace "upload_port = .*", "upload_port = $usbDevice"
      $updatedContent | Set-Content platformio.ini
      
      Write-Status "Updated platformio.ini with upload_port = $usbDevice"
    }
  }
  else {
    Write-Warning "No ESP8266 device detected"
    Write-Host "Connect your ESP8266 board via USB and run: pio device list"
    Write-Host "Then manually update upload_port in platformio.ini"
  }
}
catch {
  Write-Warning "Could not check for devices"
}

# Final instructions
Write-Host ""
Write-Host "🎉 Setup Complete!" -ForegroundColor Green
Write-Host "==================" -ForegroundColor Green
Write-Host ""
Write-Host "Next steps:"
Write-Host "1. Connect your ESP8266 board via USB"
Write-Host "2. Run: pio run --target upload"
Write-Host "3. Open Serial Monitor: pio device monitor --baud 115200"
Write-Host ""
Write-Host "Note: If Python or PlatformIO was installed, restart your terminal or IDE"
Write-Host ""
Write-Host "For workshop materials:"
Write-Host "- Read docs/exercise1.md for LED basics"
Write-Host "- Check docs/api-spec.md for API documentation"
Write-Host "- Try examples in /examples directory"
Write-Host ""
Write-Host "Troubleshooting:"
Write-Host "- If upload fails: Hold RESET, press BOOT, release RESET, release BOOT"
Write-Host "- If clangd errors: Restart VS Code/Cursor IDE"
Write-Host "- If device not found: Install CH340 or CP2102 drivers"
Write-Host ""
Write-Status "Setup script completed successfully!"
