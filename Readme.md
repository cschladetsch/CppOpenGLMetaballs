# Blob Simulation

A C++23 metaball blob simulation using SFML, Boost, and OpenGL shaders. Features fluid-like blobs that morph and merge together while conserving mass, creating organic animations with physics-based movement.

## Features

- **Metaball Rendering**: Blobs morph together smoothly using GPU shaders
- **Constant Density**: All blobs maintain the same density (mass = density × π × radius²)
- **Verlet Integration**: Stable physics simulation at 60Hz
- **Mass Conservation**: Total mass is preserved when blobs merge
- **Color Blending**: Smooth color mixing based on blob influence
- **Wrap-Around Boundaries**: Seamless screen edge wrapping
- **Dynamic Morphing**: Non-circular organic shapes during interactions
- **Gravitational Attraction**: Blobs attract each other based on mass

## Requirements

- C++23 compatible compiler (Clang++ by default, GCC supported)
- CMake 3.20+
- Boost 1.70+
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
1. Configure with CMake
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
- **Gravity**: Blobs attract with force = G × m₁ × m₂ / r²
- **Collisions**: Elastic collisions with 0.5 restitution
- **Merging**: Blobs merge when overlapping by 40% or more
- **Integration**: Verlet integration with 0.995 damping factor

### Rendering
- **Metaball Shader**: GPU-based metaball equation for smooth morphing
- **Influence Function**: f(x,y) = r² / ((x-cx)² + (y-cy)²)
- **Surface Threshold**: Configurable threshold for blob boundaries
- **Color Blending**: Weighted average based on metaball influence

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
- `gravityStrength`: Attraction force between blobs
- `minDistance`: Minimum distance for force calculations
- `radiusDist`: Range of blob sizes (10-40 pixels)

### Tweaking Visuals
Edit `Shaders/metaball.frag`:
- `threshold`: Surface definition threshold
- Edge smoothing parameters
- Color blending algorithm

## License

This project is provided as-is for educational and entertainment purposes.