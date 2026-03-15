message(STATUS "Configuring kEn")

# NOTE: Adding new .cpp files requires re-running CMake (GLOB_RECURSE does not
# trigger automatic reconfiguration).
file(GLOB_RECURSE SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_library(kEn ${SOURCES})

# Prevent CMake from adding `lib` before `kEn`
set_target_properties(kEn PROPERTIES PREFIX "")

target_compile_features(kEn PRIVATE cxx_std_23)

# GLFW_INCLUDE_NONE makes the GLFW header not include any OpenGL or OpenGL ES
# API header. This is useful in combination with an extension loading library
# (GLAD in our case).
target_compile_definitions(kEn PUBLIC GLFW_INCLUDE_NONE GLM_ENABLE_EXPERIMENTAL STB_IMAGE_IMPLEMENTATION)

target_include_directories(kEn
        PUBLIC src "${CMAKE_BINARY_DIR}/generated")

target_link_libraries(kEn
    PUBLIC  mEn glfw imgui imguizmo
    PRIVATE glad assimp spdlog stb nfd)

# Set compile options and properties of the target
target_compile_options(kEn PRIVATE ${PROJ_CXX_FLAGS})

# This is required when kEn is built as a shared library and is linked with shared libraries
target_link_options(kEn PRIVATE ${PROJ_SHARED_LINKER_FLAGS})

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
