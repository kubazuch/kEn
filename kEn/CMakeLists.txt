project(kEn CXX C)

cmake_minimum_required(VERSION 3.20)
include(../cmake/dependencies.cmake)
include(../cmake/configure_library.cmake)

option(BUILD_TESTS "Fetch GoogleTest and build tests" OFF)
option(USE_SYSTEM_INCLUDE "Marks includes as system" ON)

fetch_glfw()
fetch_spdlog()
fetch_imgui()
fetch_imguizmo()
fetch_glad()
fetch_assimp()
fetch_stb()
fetch_nfd()

if(WIN32)
    list(APPEND KEN_COMPILE_DEFS KEN_PLATFORM_WIN)
endif()

configure_library(
    NAME kEn
    DEPS_PUBLIC mEn glfw spdlog imgui imguizmo glad assimp stb nfd
    COMPILE_DEFINITIONS
        GLFW_INCLUDE_NONE
        STB_IMAGE_IMPLEMENTATION
        ${KEN_COMPILE_DEFS}
        $<$<CONFIG:Debug>:_KEN_DEBUG>
)

# ##############################################################################
# Shader embedding
# ##############################################################################
# Set shader source directory and generated output directory.
set(SHADER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/assets/shaders")
set(GENERATED_SHADERS_DIR "${CMAKE_BINARY_DIR}/generated")

# Include the reusable shader-embedding module (assumed to be in the same directory)
include(../cmake/embed_shaders.cmake)
embed_shaders(${SHADER_DIR} ${GENERATED_SHADERS_DIR})
add_dependencies(kEn EmbedShaders)
