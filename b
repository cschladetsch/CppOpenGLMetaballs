#!/bin/bash

# Build script for Blob Simulation

# Colors for output
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color

# Default to clang++
USE_CLANG=true

# Parse arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        --gcc)
            USE_CLANG=false
            shift
            ;;
        --help)
            echo "Usage: ./b [options]"
            echo "Options:"
            echo "  --gcc     Use GCC instead of Clang"
            echo "  --help    Show this help message"
            exit 0
            ;;
        *)
            echo "Unknown option: $1"
            echo "Use --help for usage information"
            exit 1
            ;;
    esac
done

# Set compiler
if [ "$USE_CLANG" = true ]; then
    export CC=clang
    export CXX=clang++
    echo "Using Clang++ compiler"
else
    export CC=gcc
    export CXX=g++
    echo "Using G++ compiler"
fi

echo "Building Blob Simulation..."

# Create build directory if it doesn't exist
if [ ! -d "build" ]; then
    mkdir build
fi

cd build

# Configure with CMake
echo "Configuring with CMake..."
cmake -G Ninja .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=$CC -DCMAKE_CXX_COMPILER=$CXX

if [ $? -ne 0 ]; then
    echo -e "${RED}CMake configuration failed!${NC}"
    exit 1
fi

# Build
echo "Building project..."
ninja

if [ $? -ne 0 ]; then
    echo -e "${RED}Build failed!${NC}"
    exit 1
fi

echo -e "${GREEN}Build successful!${NC}"

# Run tests
echo "Running tests..."
./blob_tests

if [ $? -ne 0 ]; then
    echo -e "${RED}Tests failed!${NC}"
    exit 1
fi

echo -e "${GREEN}All tests passed!${NC}"