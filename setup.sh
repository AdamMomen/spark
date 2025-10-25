#!/bin/bash

# IoT Workshop Setup Script
# Cross-platform setup for macOS and Linux
# Automates the setup process from setup.md

set -e  # Exit on any error

echo "🚀 IoT Workshop Setup Script (Cross-Platform)"
echo "============================================="

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Function to print colored output
print_status() {
    echo -e "${GREEN}✅ $1${NC}"
}

print_warning() {
    echo -e "${YELLOW}⚠️  $1${NC}"
}

print_error() {
    echo -e "${RED}❌ $1${NC}"
}

print_info() {
    echo -e "${BLUE}ℹ️  $1${NC}"
}

# Detect operating system
detect_os() {
    if [[ "$OSTYPE" == "darwin"* ]]; then
        echo "macos"
    elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
        echo "linux"
    else
        echo "unknown"
    fi
}

# Detect Linux distribution
detect_distro() {
    if [ -f /etc/os-release ]; then
        . /etc/os-release
        DISTRO=$ID
        DISTRO_VERSION=$VERSION_ID
    elif [ -f /etc/debian_version ]; then
        DISTRO="debian"
    elif [ -f /etc/redhat-release ]; then
        DISTRO="rhel"
    elif [ -f /etc/arch-release ]; then
        DISTRO="arch"
    else
        DISTRO="unknown"
    fi
    echo $DISTRO
}

# Install system dependencies for Linux
install_linux_deps() {
    local distro=$1
    print_info "Installing system dependencies for $distro..."
    
    case $distro in
        ubuntu|debian)
            sudo apt update
            sudo apt install -y curl git build-essential libssl-dev zlib1g-dev \
                libbz2-dev libreadline-dev libsqlite3-dev wget llvm libncurses5-dev \
                libncursesw5-dev xz-utils tk-dev libffi-dev liblzma-dev python3-openssl
            ;;
        fedora|rhel|centos)
            sudo dnf install -y curl git gcc gcc-c++ make openssl-devel zlib-devel \
                bzip2-devel readline-devel sqlite-devel wget llvm ncurses-devel \
                tk-devel libffi-devel xz-devel python3-openssl
            ;;
        arch|manjaro)
            sudo pacman -S --noconfirm curl git base-devel openssl zlib bzip2 \
                readline sqlite wget llvm ncurses tk libffi xz python-openssl
            ;;
        opensuse*)
            sudo zypper install -y curl git gcc gcc-c++ make libopenssl-devel \
                zlib-devel bzip2-devel readline-devel sqlite3-devel wget llvm \
                ncurses-devel tk-devel libffi-devel xz-devel python3-openssl
            ;;
        *)
            print_warning "Unknown distribution: $distro"
            print_info "Please install these packages manually:"
            echo "  - curl, git, build-essential/gcc, libssl-dev, zlib1g-dev"
            echo "  - libbz2-dev, libreadline-dev, libsqlite3-dev, wget"
            echo "  - llvm, libncurses5-dev, libncursesw5-dev, xz-utils"
            echo "  - tk-dev, libffi-dev, liblzma-dev, python3-openssl"
            ;;
    esac
}

# Install pyenv (Linux only)
install_pyenv() {
    if command -v pyenv &> /dev/null; then
        PYENV_VERSION=$(pyenv --version | cut -d' ' -f2)
        print_status "pyenv $PYENV_VERSION found"
        return 0
    fi
    
    print_warning "pyenv not found. Installing pyenv..."
    
    # Install pyenv
    curl https://pyenv.run | bash
    
    # Add pyenv to shell configuration
    if [[ "$SHELL" == *"zsh"* ]]; then
        SHELL_CONFIG="$HOME/.zshrc"
    elif [[ "$SHELL" == *"bash"* ]]; then
        SHELL_CONFIG="$HOME/.bashrc"
    else
        SHELL_CONFIG="$HOME/.profile"
    fi
    
    echo 'export PYENV_ROOT="$HOME/.pyenv"' >> "$SHELL_CONFIG"
    echo 'command -v pyenv >/dev/null || export PATH="$PYENV_ROOT/bin:$PATH"' >> "$SHELL_CONFIG"
    echo 'eval "$(pyenv init -)"' >> "$SHELL_CONFIG"
    
    # Source the configuration
    export PYENV_ROOT="$HOME/.pyenv"
    export PATH="$PYENV_ROOT/bin:$PATH"
    eval "$(pyenv init -)"
    
    print_status "pyenv installed successfully"
    print_info "Added pyenv to $SHELL_CONFIG"
}

# Install Python using pyenv (Linux) or system Python (macOS)
install_python() {
    local os=$1
    
    if command -v python3 &> /dev/null; then
        PYTHON_VERSION=$(python3 --version | cut -d' ' -f2)
        print_status "Python $PYTHON_VERSION found"
        
        # Check if it's a good version (3.7+)
        if python3 -c "import sys; exit(0 if sys.version_info >= (3, 7) else 1)" 2>/dev/null; then
            return 0
        else
            print_warning "Python version is too old"
        fi
    else
        print_warning "Python not found"
    fi
    
    if [[ "$os" == "linux" ]]; then
        # Use pyenv for Linux
        local python_version="3.11.0"
        print_warning "Installing Python $python_version via pyenv..."
        
        pyenv install $python_version
        pyenv global $python_version
        
        # Refresh PATH
        export PATH="$PYENV_ROOT/bin:$PATH"
        eval "$(pyenv init -)"
        
        # Verify installation
        PYTHON_VERSION=$(python3 --version | cut -d' ' -f2)
        print_status "Python $PYTHON_VERSION now available"
    else
        # Use Homebrew for macOS
        print_warning "Installing Python via Homebrew..."
        brew install python
        print_status "Python installed successfully"
        
        # Verify installation
        PYTHON_VERSION=$(python3 --version | cut -d' ' -f2)
        print_status "Python $PYTHON_VERSION now available"
    fi
}

# Install ESP8266 drivers (Linux specific)
install_linux_esp8266_drivers() {
    print_info "Setting up ESP8266 drivers for Linux..."
    
    # Add user to dialout group for serial access
    if ! groups $USER | grep -q dialout; then
        print_warning "Adding user to dialout group for serial access..."
        sudo usermod -a -G dialout $USER
        print_status "User added to dialout group"
        print_warning "You may need to log out and back in for group changes to take effect"
    else
        print_status "User already in dialout group"
    fi
    
    # Install udev rules for ESP8266
    print_info "Installing udev rules for ESP8266..."
    sudo tee /etc/udev/rules.d/99-esp8266.rules > /dev/null <<EOF
# ESP8266 NodeMCU
SUBSYSTEM=="usb", ATTRS{idVendor}=="10c4", ATTRS{idProduct}=="ea60", MODE="0666", GROUP="dialout"
SUBSYSTEM=="usb", ATTRS{idVendor}=="1a86", ATTRS{idProduct}=="7523", MODE="0666", GROUP="dialout"
SUBSYSTEM=="usb", ATTRS{idVendor}=="0403", ATTRS{idProduct}=="6001", MODE="0666", GROUP="dialout"
EOF
    
    # Reload udev rules
    sudo udevadm control --reload-rules
    sudo udevadm trigger
    
    print_status "ESP8266 udev rules installed"
}

# Detect operating system
OS=$(detect_os)
if [[ "$OS" == "unknown" ]]; then
    print_error "This script supports macOS and Linux only"
    exit 1
fi

print_status "Detected $OS system"

# Step 1: Install system dependencies
echo ""
echo "📋 Step 1: Installing system dependencies..."
if [[ "$OS" == "linux" ]]; then
    DISTRO=$(detect_distro)
    print_info "Detected distribution: $DISTRO"
    install_linux_deps $DISTRO
elif [[ "$OS" == "macos" ]]; then
    # Check and install Homebrew
    if command -v brew &> /dev/null; then
        BREW_VERSION=$(brew --version | head -n1 | cut -d' ' -f2)
        print_status "Homebrew $BREW_VERSION found"
    else
        print_warning "Homebrew not found. Installing Homebrew..."
        /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
        
        # Add Homebrew to PATH for Apple Silicon Macs
        if [[ $(uname -m) == "arm64" ]]; then
            # Detect shell and add to appropriate config file
            if [[ "$SHELL" == *"zsh"* ]]; then
                SHELL_CONFIG="$HOME/.zshrc"
            elif [[ "$SHELL" == *"bash"* ]]; then
                SHELL_CONFIG="$HOME/.bash_profile"
            else
                SHELL_CONFIG="$HOME/.profile"
            fi
            
            echo 'eval "$(/opt/homebrew/bin/brew shellenv)"' >> "$SHELL_CONFIG"
            eval "$(/opt/homebrew/bin/brew shellenv)"
            print_status "Added Homebrew to PATH in $SHELL_CONFIG"
        fi
        
        print_status "Homebrew installed successfully"
    fi
fi

# Step 2: Install pyenv (Linux only)
if [[ "$OS" == "linux" ]]; then
    echo ""
    echo "📋 Step 2: Installing pyenv..."
    install_pyenv
fi

# Step 3: Check and install Python
echo ""
echo "📋 Step 3: Checking Python installation..."
install_python $OS

# Step 4: Install PlatformIO
echo ""
echo "📋 Step 4: Installing PlatformIO..."
if command -v pio &> /dev/null; then
    PIO_VERSION=$(pio --version)
    print_status "PlatformIO already installed: $PIO_VERSION"
else
    print_warning "Installing PlatformIO via pip..."
    pip3 install platformio
    print_status "PlatformIO installed successfully"
fi

# Step 5: Install ESP8266 drivers
echo ""
echo "📋 Step 5: Installing ESP8266 drivers..."
if [[ "$OS" == "linux" ]]; then
    install_linux_esp8266_drivers
elif [[ "$OS" == "macos" ]]; then
    print_warning "If you encounter 'device not found' errors, run:"
    echo "  brew install --cask wch-ch34x-usb-serial-driver"
fi

# Step 6: Setup clangd configuration
echo ""
echo "📋 Step 6: Setting up clangd configuration..."
if [ -f ".clang_template" ]; then
    print_status "Found .clang_template"
    # Generate compile_commands.json for better clangd support
    print_warning "Generating compile_commands.json..."
    pio run --target compiledb 2>/dev/null || print_warning "CompileDB generation failed (this is optional)"
    print_status "clangd configuration ready"
else
    print_warning ".clang_template not found, using default clangd config"
fi

# Step 7: Test compilation
echo ""
echo "📋 Step 7: Testing compilation..."
if pio run --target checkprogsize &> /dev/null; then
    print_status "Compilation test passed"
else
    print_warning "Compilation test failed - this is normal if no code is uploaded yet"
fi

# Step 8: Check for ESP8266 device and update platformio.ini
echo ""
echo "📋 Step 8: Checking for ESP8266 devices..."
if pio device list | grep -q "usb"; then
    print_status "ESP8266 device detected"
    echo "Available devices:"
    pio device list | grep "usb"
    
    # Find the first USB device and update platformio.ini
    USB_DEVICE=$(pio device list | grep "usb" | head -n1 | awk '{print $1}')
    if [[ -n "$USB_DEVICE" ]]; then
        print_info "Updating platformio.ini with device: $USB_DEVICE"
        
        # Update platformio.ini with the detected USB device
        if [[ "$OS" == "linux" ]]; then
            # Linux devices are typically /dev/ttyUSB* or /dev/ttyACM*
            sed -i "s|upload_port = .*|upload_port = $USB_DEVICE|" platformio.ini
        elif [[ "$OS" == "macos" ]]; then
            # macOS devices are typically /dev/cu.usbserial-* or /dev/cu.SLAB_USBtoUART*
            sed -i '' "s|upload_port = .*|upload_port = $USB_DEVICE|" platformio.ini
        fi
        
        print_status "Updated platformio.ini with upload_port = $USB_DEVICE"
    fi
    
else
    print_warning "No ESP8266 device detected"
    echo "Connect your ESP8266 board via USB and run: pio device list"
    echo "Then manually update upload_port in platformio.ini"
fi

# Final instructions
echo ""
echo "🎉 Setup Complete!"
echo "=================="
echo ""
echo "Next steps:"
echo "1. Connect your ESP8266 board via USB"
echo "2. Run: pio run --target upload"
echo "3. Open Serial Monitor: pio device monitor --baud 115200"
echo ""
if [[ "$OS" == "linux" ]]; then
    echo "Note: If you were added to the dialout group, you may need to:"
    echo "  - Log out and back in, OR"
    echo "  - Run: newgrp dialout"
elif [[ "$OS" == "macos" ]]; then
    echo "Note: If Homebrew was installed, restart your terminal or run:"
    if [[ "$SHELL" == *"zsh"* ]]; then
        echo "  source ~/.zshrc"
    elif [[ "$SHELL" == *"bash"* ]]; then
        echo "  source ~/.bash_profile"
    else
        echo "  source ~/.profile"
    fi
fi
echo ""
echo "For workshop materials:"
echo "- Read docs/exercise1.md for LED basics"
echo "- Check docs/api-spec.md for API documentation"
echo "- Try examples in /examples directory"
echo ""
echo "Troubleshooting:"
echo "- If upload fails: Hold RESET, press BOOT, release RESET, release BOOT"
echo "- If clangd errors: Restart VS Code/Cursor IDE"
if [[ "$OS" == "linux" ]]; then
    echo "- If permission denied: Check dialout group membership"
    echo "- If device not found: Check udev rules and USB connection"
elif [[ "$OS" == "macos" ]]; then
    echo "- If device not found: Install CH340 drivers with brew"
fi
echo ""
print_status "Setup script completed successfully!"
