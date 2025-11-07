cmake_minimum_required(VERSION 3.1)
project(Raytracer CXX)

set(CMAKE_CXX_STANDARD 20)

# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

add_subdirectory(third-party/SDL EXCLUDE_FROM_ALL)
file(GLOB SRC_FILES src/*.cpp)
add_executable(Raytracer ${SRC_FILES})

target_include_directories(Raytracer PRIVATE src)
target_link_libraries(Raytracer PRIVATE SDL3::SDL3)