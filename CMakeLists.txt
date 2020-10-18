cmake_minimum_required(VERSION 3.12)

project(InfinityClimb VERSION 1.0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Files
set(IC_ROOT "${CMAKE_CURRENT_SOURCE_DIR}")
set(IC_DEPS "${IC_ROOT}/deps")

set(sources
    "Main.c"
    "Libs.c"
    "Log.c"
    "util/Utils.c"
    "util/Color.c"
    "util/String.c"
    "util/Vector.c"
    "util/math/Vec.c"
    "util/math/Mat.c"
    "window/Window.c"
    "window/Input.c"
    "window/Context.c"
    "world/Camera.c"
    "world/model/Shader.c"
    "world/model/mesh/Mesh.c"
    "world/model/mesh/Mesh_Shader.c"
    "world/model/skybox/Skybox.c"
    "world/model/skybox/Skybox_Shader.c"
    "world/player/Player.c"
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

include_directories("${IC_DEPS}/stb")

# Build
set(EXECUTABLE_OUTPUT_PATH "${IC_ROOT}/bin")
add_executable(InfinityClimb ${sources})
target_link_libraries(InfinityClimb glfw glad)
