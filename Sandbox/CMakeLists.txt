message(STATUS "Configuring Sandbox")

# NOTE: Adding new .cpp files requires re-running CMake (GLOB_RECURSE does not
# trigger automatic reconfiguration).
file(GLOB_RECURSE SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_executable(Sandbox ${SOURCES})

target_link_libraries(Sandbox PRIVATE kEn)
target_include_directories(Sandbox
                           PRIVATE src "${CMAKE_BINARY_DIR}/generated")

# Set compile options and properties of the target
target_compile_options(Sandbox PRIVATE ${PROJ_CXX_FLAGS})

# This is required when Sandbox is linked with shared libraries
target_link_options(Sandbox PRIVATE ${PROJ_EXE_LINKER_FLAGS})

# Copy assets folder to build directory
add_custom_target(copy_assets
  COMMAND ${CMAKE_COMMAND} -DASSETS_SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}/assets
                           -DASSETS_DEST_DIR=$<TARGET_FILE_DIR:Sandbox>
                           -P ${CMAKE_CURRENT_SOURCE_DIR}/../cmake/copy_assets.cmake
)
add_dependencies(Sandbox copy_assets)
