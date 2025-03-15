cmake_minimum_required(VERSION 3.20)

project(kEn)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
message(STATUS "Configuring " ${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 23)

file(GLOB_RECURSE SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

# Enable Windows-specific settings
if (WIN32)
    add_definitions(-D_WIN32_WINNT=0x0601)  # Target Windows 7 or later (modify as needed)
    add_definitions(-DUNICODE -D_UNICODE)   # Enable Unicode support
endif()

add_library(${PROJECT_NAME} ${SOURCES})

# Prevent CMake from adding `lib` before `kEn`
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")

target_include_directories(${PROJECT_NAME}
        PUBLIC src "${CMAKE_BINARY_DIR}/generated")
target_link_libraries(${PROJECT_NAME} PUBLIC mEn glad glfw assimp spdlog stb imgui imguizmo user32 kernel32 gdi32)

# Set compile options and properties of the target
target_compile_options(${PROJECT_NAME} PRIVATE ${PROJ_CXX_FLAGS})

# This is required when kEn is built as a shared library and is linked with shared libraries
target_link_options(${PROJECT_NAME} PRIVATE ${PROJ_SHARED_LINKER_FLAGS})

target_compile_definitions(${PROJECT_NAME} PUBLIC GLM_ENABLE_EXPERIMENTAL STB_IMAGE_IMPLEMENTATION)

target_precompile_headers(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/kenpch.hpp")