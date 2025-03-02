cmake_minimum_required(VERSION 3.20)

project(Sandbox CXX)
message(STATUS "Configuring " ${PROJECT_NAME})
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)

file(GLOB_RECURSE SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PUBLIC kEn)
target_include_directories(${PROJECT_NAME}
                           PUBLIC src "${CMAKE_BINARY_DIR}/generated")

# GLFW_INCLUDE_NONE makes the GLFW header not include any OpenGL or OpenGL ES
# API header. This is useful in combination with an extension loading library
# (GLAD in our case).
target_compile_definitions(${PROJECT_NAME} PUBLIC GLFW_INCLUDE_NONE)

# Set compile options and properties of the target
target_compile_options(${PROJECT_NAME} PRIVATE ${PROJ_CXX_FLAGS})

# This is required when Sandbox is linked with shared libraries
target_link_options(${PROJECT_NAME} PRIVATE ${PROJ_EXE_LINKER_FLAGS})

# Copy assets folder to build directory
add_custom_target(copy_assets
  COMMAND ${CMAKE_COMMAND} -DASSETS_SOURCE_DIR=${PROJECT_SOURCE_DIR}/assets
                           -DASSETS_DEST_DIR=$<TARGET_FILE_DIR:${PROJECT_NAME}>
                           -P ${PROJECT_SOURCE_DIR}/../cmake/copy_assets.cmake
)
add_dependencies(${PROJECT_NAME} copy_assets)
