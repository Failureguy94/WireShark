#!/bin/bash

# WireShark Project Build and Setup Script

set -e

echo "=== WireShark Project Setup ==="

# Check if we're in the correct directory
if [ ! -f "Readme.md" ]; then
    echo "Error: Please run this script from the project root directory"
    exit 1
fi

# Function to check if command exists
command_exists() {
    command -v "$1" >/dev/null 2>&1
}

# Check dependencies
echo "Checking dependencies..."

# Check for cmake
if ! command_exists cmake; then
    echo "Error: cmake not found. Please install cmake"
    echo "Ubuntu/Debian: sudo apt-get install cmake"
    echo "macOS: brew install cmake"
    exit 1
fi

# Check for make
if ! command_exists make; then
    echo "Error: make not found. Please install build-essential"
    echo "Ubuntu/Debian: sudo apt-get install build-essential"
    exit 1
fi

# Check for pkg-config
if ! command_exists pkg-config; then
    echo "Error: pkg-config not found. Please install pkg-config"
    echo "Ubuntu/Debian: sudo apt-get install pkg-config"
    exit 1
fi

# Check for libpcap development files
if ! pkg-config --exists libpcap; then
    echo "Error: libpcap development files not found"
    echo "Ubuntu/Debian: sudo apt-get install libpcap-dev"
    echo "macOS: brew install libpcap"
    exit 1
fi

# Check for Node.js and npm
if ! command_exists node; then
    echo "Error: Node.js not found. Please install Node.js"
    echo "Visit: https://nodejs.org/"
    exit 1
fi

if ! command_exists npm; then
    echo "Error: npm not found. Please install npm"
    exit 1
fi

echo "✓ All dependencies found"

# Build C++ backend
echo ""
echo "Building C++ backend..."
cd WireShark/cpp-packet-analyzer

# Create build directory
if [ ! -d "build" ]; then
    mkdir build
fi

cd build

# Configure with CMake
cmake ..

# Build the project
make -j$(nproc 2>/dev/null || echo 2)

if [ $? -eq 0 ]; then
    echo "✓ C++ backend built successfully"
else
    echo "✗ C++ backend build failed"
    exit 1
fi

# Go back to project root
cd ../../..

# Setup React frontend
echo ""
echo "Setting up React frontend..."
cd frontend

# Install npm dependencies
npm install

if [ $? -eq 0 ]; then
    echo "✓ Frontend dependencies installed successfully"
else
    echo "✗ Frontend dependency installation failed"
    exit 1
fi

# Go back to project root
cd ..

echo ""
echo "=== Setup Complete! ==="
echo ""
echo "To run the project:"
echo "1. Start the C++ backend:"
echo "   cd WireShark/cpp-packet-analyzer/build"
echo "   sudo ./packet_analyzer"
echo ""
echo "2. In another terminal, start the React frontend:"
echo "   cd frontend"
echo "   npm start"
echo ""
echo "3. Open your browser to: http://localhost:3000"
echo ""
echo "Note: The C++ backend requires sudo privileges for packet capture"
echo "The backend API will be available at: http://localhost:8080"