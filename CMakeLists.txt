cmake_minimum_required(VERSION 3.12)

project(InfinityClimb VERSION 1.0)

# Files
set(IC_ROOT "${CMAKE_CURRENT_SOURCE_DIR}")
set(IC_DEPS "${IC_ROOT}/deps")

set(sources
    "Main.c"
    "Libs.c"
    "Log.c"
    "window/Window.c"
    "window/Context.c"
    "world/Rect.c"
    "world/Shader.c"
)

list(TRANSFORM sources PREPEND "${IC_ROOT}/src/")
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
