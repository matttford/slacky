#!/bin/bash

# Development script for easy compile and run
# Usage: ./dev.sh

set -e  # Exit on any error

echo "🔨 Building project..."

# Navigate to build directory
cd build

# Build the project
../vcpkg/downloads/tools/cmake-3.30.1-osx/cmake-3.30.1-macos-universal/CMake.app/Contents/bin/cmake --build .

echo "✅ Build complete!"

echo "🚀 Running slacky_test..."
echo "----------------------------------------"

# Run the executable
./bin/slacky_test

echo "----------------------------------------"
echo "✨ Done!"