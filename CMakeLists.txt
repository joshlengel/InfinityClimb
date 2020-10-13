cmake_minimum_required(VERSION 3.10)

project(InfinityClimb VERSION 1.0)

# Files
set(IC_ROOT "${CMAKE_CURRENT_SOURCE_DIR}")
set(IC_DEPS "${IC_ROOT}/deps")

file(GLOB_RECURSE sources "${IC_ROOT}/src/**.cpp")
include_directories("${IC_ROOT}/include" "${IC_ROOT}/src")

# Dependencies
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
add_subdirectory("${IC_DEPS}/glfw")
include_directories("${IC_DEPS}/glfw/include")

add_subdirectory("${IC_DEPS}/glad")
include_directories("${IC_DEPS}/glad/include")

# Build
set(EXECUTABLE_OUTPUT_PATH "${IC_ROOT}/bin")
add_executable(InfinityClimb ${sources})
target_link_libraries(InfinityClimb glfw glad)
