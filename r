#!/bin/bash

# Run script for Blob Simulation

# Default values
WIDTH=1280
HEIGHT=720
BUILD=true
TESTS=true

# Parse arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        -w|--width)
            WIDTH="$2"
            shift 2
            ;;
        -h|--height)
            HEIGHT="$2"
            shift 2
            ;;
        --no-build)
            BUILD=false
            shift
            ;;
        --no-tests)
            TESTS=false
            shift
            ;;
        --help)
            echo "Usage: ./r [options]"
            echo "Options:"
            echo "  -w, --width WIDTH      Set window width (default: 1280)"
            echo "  -h, --height HEIGHT    Set window height (default: 720)"
            echo "  --no-build            Skip building"
            echo "  --no-tests            Skip running tests"
            echo "  --help                Show this help message"
            echo ""
            echo "Examples:"
            echo "  ./r                   Run with defaults"
            echo "  ./r -w 1920 -h 1080   Run in full HD"
            echo "  ./r --no-build        Run without rebuilding"
            exit 0
            ;;
        *)
            echo "Unknown option: $1"
            echo "Use --help for usage information"
            exit 1
            ;;
    esac
done

# Build if requested
if [ "$BUILD" = true ]; then
    echo "Building project..."
    ./b
    if [ $? -ne 0 ]; then
        echo "Build failed!"
        exit 1
    fi
else
    # Skip tests if not building
    TESTS=false
fi

# Run simulation
echo "Starting Blob Simulation (${WIDTH}x${HEIGHT})..."
echo "Controls:"
echo "  Space - Add new random blob"
echo "  R     - Reset simulation"
echo "  ESC   - Exit"
echo ""

cd build
./blob_sim $WIDTH $HEIGHT