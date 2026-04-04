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

# ##############################################################################
# Vendor: mikktspace (C library)
# ##############################################################################
add_library(mikktspace STATIC
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/mikktspace.c"
)
target_include_directories(mikktspace PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor"
)

if(WIN32)
    list(APPEND KEN_COMPILE_DEFS KEN_PLATFORM_WIN)
endif()

configure_library(
    NAME kEn
    DEPS_PUBLIC mEn glfw spdlog imgui imguizmo glad assimp stb nfd mikktspace
    COMPILE_DEFINITIONS
        GLFW_INCLUDE_NONE
        STB_IMAGE_IMPLEMENTATION
        ${KEN_COMPILE_DEFS}
        $<$<CONFIG:Debug>:KEN_DEBUG_BUILD>
)

# ##############################################################################
# Shader embedding
# ##############################################################################
# Set shader source directory and generated output directory.
set(SHADER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/assets/shaders")
set(GENERATED_SHADERS_DIR "${CMAKE_BINARY_DIR}/generated")

# Include the reusable shader-embedding module (assumed to be in the same directory)
include(../cmake/embed_shaders.cmake)
embed_shaders(
    NAME kEn
    SHADER_DIR "${SHADER_DIR}"
    GENERATED_DIR "${GENERATED_SHADERS_DIR}"
)
add_dependencies(kEn ${EMBED_SHADERS_TARGET})

# Convenience target for CI: generates all headers needed for clang-tidy
# without compiling any kEn/mEn/Sandbox C++ sources.
add_custom_target(generate_all DEPENDS ${EMBED_SHADERS_TARGET} glad)
