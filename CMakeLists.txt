cmake_minimum_required(VERSION 3.20)
project(BlobSimulation LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# SFML
set(SFML_BUILD_AUDIO OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.1
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(SFML)

# OpenGL
find_package(OpenGL REQUIRED)

# Boost
find_package(Boost 1.70 REQUIRED COMPONENTS system)

# Google Test
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
    GIT_SHALLOW TRUE
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Main executable
add_executable(blob_sim 
    Source/main.cpp
    Source/Blob.cpp
    Source/BlobSimulation.cpp
    Source/ShaderManager.cpp
)

target_link_libraries(blob_sim 
    sfml-graphics 
    sfml-window 
    sfml-system
    Boost::system
    ${OPENGL_LIBRARIES}
)

target_include_directories(blob_sim PRIVATE Source)

# Copy shaders to build directory
file(COPY Shaders DESTINATION ${CMAKE_BINARY_DIR})

# Tests
enable_testing()
add_executable(blob_tests
    Tests/blob_tests.cpp
    Source/Blob.cpp
)

target_link_libraries(blob_tests
    gtest_main
    sfml-graphics
    sfml-system
)

target_include_directories(blob_tests PRIVATE Source)

include(GoogleTest)
gtest_discover_tests(blob_tests)