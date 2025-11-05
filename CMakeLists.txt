cmake_minimum_required(VERSION 3.30)
project(Raytracer C)

set(CMAKE_C_STANDARD 11)

# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

add_subdirectory(third-party/SDL EXCLUDE_FROM_ALL)
add_executable(Raytracer main.c)

# Link to the actual SDL3 library.
target_link_libraries(Raytracer PRIVATE SDL3::SDL3)