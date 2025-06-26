# Blob Simulation

A C++23 metaball blob simulation using SFML and OpenGL shaders. Features fluid-like blobs that smoothly morph together using GPU-accelerated metaball rendering, creating organic animations with gravitational physics at 60Hz.

## Demo

![Demo](Resources/Demo1.gif)

## Features

- **Smooth Metaball Morphing**: GPU-accelerated shader creates organic blob fusion without discrete merging
- **Transparent Edges**: Blobs fade smoothly to transparency with alpha blending
- **Gravitational Physics**: Mass-based attraction with stronger forces when blobs are close
- **Constant Density**: All blobs maintain uniform density (mass = density × π × radius²)
- **Verlet Integration**: Stable physics simulation at 60Hz with minimal damping
- **Wrap-Around Boundaries**: Seamless screen edge wrapping with proper force calculations
- **Dynamic Color Blending**: Colors mix based on metaball influence in the shader
- **Gentle Collision Response**: Minimal separation prevents complete overlap while allowing morphing

## Requirements

- C++23 compatible compiler (Clang++ by default, GCC supported)
- CMake 3.20+
- Ninja build system
- OpenGL 3.3+
- X11 (Linux)

## Building

```bash
./b [options]
```

Options:
- `--gcc` - Use GCC instead of Clang++ (default)
- `--help` - Show build options

The build script will:
1. Configure with CMake using Ninja generator
2. Compile with Clang++ (or GCC if specified)
3. Run all unit tests
4. Copy shader files to build directory

## Running

```bash
./r [options]
```

Options:
- `-w, --width WIDTH` - Window width in pixels (default: 1280)
- `-h, --height HEIGHT` - Window height in pixels (default: 720)
- `--no-build` - Skip rebuilding before running
- `--no-tests` - Skip running tests
- `--help` - Show all options

Examples:
```bash
./r                    # Run with default settings
./r -w 1920 -h 1080    # Run in Full HD
./r --no-build         # Quick run without rebuilding
```

## Controls

- **Space** - Add a new random blob
- **R** - Reset simulation with fresh blobs
- **ESC** - Exit application

## Implementation Details

### Physics
- **Density**: All blobs have uniform density of 1.0
- **Gravity**: Strong attraction with force = 2000 × m₁ × m₂ / r²
- **Close-Range Forces**: Double attraction when blobs are within 1.5× combined radii
- **Collision Response**: Minimal separation (2% of overlap) to allow visual morphing
- **No Discrete Merging**: Blobs maintain individual physics while visually morphing
- **Integration**: Verlet integration with 0.995 damping factor

### Rendering
- **Metaball Shader**: Smooth influence functions with extended falloff range
- **Influence Function**: Quadratic core with cubic falloff for organic shapes
- **Alpha Blending**: Transparent edges with smoothstep functions
- **Surface Threshold**: Low threshold (1.0) for smooth visual blending
- **Color Mixing**: Influence-weighted color averaging in shader
- **Render Pipeline**: Full-screen quad with per-pixel metaball evaluation

### Architecture
- **Blob Class**: Individual blob physics and properties
- **BlobSimulation**: Main simulation loop and rendering
- **ShaderManager**: Loads and manages OpenGL shaders
- **Unit Tests**: Google Test suite for physics validation

## Project Structure

```
CppBlobs/
├── Source/
│   ├── main.cpp           # Entry point
│   ├── Blob.cpp/h         # Blob physics and properties
│   ├── BlobSimulation.cpp/h # Main simulation logic
│   └── ShaderManager.cpp/h  # Shader loading and management
├── Shaders/
│   ├── blob.vert/frag     # Individual blob shaders
│   └── metaball.vert/frag # Metaball morphing shaders
├── Tests/
│   └── blob_tests.cpp     # Unit tests
├── CMakeLists.txt         # Build configuration
├── b                      # Build script
└── r                      # Run script
```

## Customization

### Adding More Blobs
Edit `Source/BlobSimulation.cpp`:
```cpp
const int numBlobs = 30; // Change this value
```

### Adjusting Physics
Modify constants in `Source/BlobSimulation.cpp`:
- `gravityStrength`: Base attraction force (currently 2000.0)
- `minDistance`: Minimum distance for force calculations (20.0)
- `radiusDist`: Range of blob sizes (10-40 pixels)
- Close-range force multiplier in `applyForces()`

### Tweaking Visuals
Edit `Shaders/metaball.frag`:
- `threshold`: Surface definition threshold (1.0 for smooth morphing)
- Influence falloff range and curve in `metaball()` function
- Alpha blending parameters for edge transparency
- Color mixing weights based on influence

## License

This project is provided as-is for educational and entertainment purposes.
