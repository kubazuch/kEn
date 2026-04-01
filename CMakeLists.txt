cmake_minimum_required(VERSION 3.30)

find_package(Git)
include(cmake/find_repo_version.cmake)
find_repo_version(VERSION IS_STABLE)

project(
  kEn
  DESCRIPTION "kEn Rendering Engine"
  LANGUAGES CXX C
  VERSION ${VERSION})

set(CMAKE_CXX_STANDARD 23)

option(GENERATE_CLANGD_FILE "Generate the .clangd file for the host operating system" ${PROJECT_IS_TOP_LEVEL})
option(BUILD_SANDBOX "Build kEn sandbox executable" OFF)

set(VERSION_SUFFIX "")
if(IS_STABLE EQUAL 0)
  set(VERSION_SUFFIX "(unstable)")
endif()
message(STATUS "Version: ${CMAKE_PROJECT_VERSION}${VERSION_SUFFIX}")

if(PROJECT_IS_TOP_LEVEL)
  # Set the binaries and libraries output directory
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

  # Include CTest in order to generate `DartConfiguration.tcl`
  include(CTest)
endif()

if(GENERATE_CLANGD_FILE)
    include(cmake/generate_clangd_file.cmake)
    generate_clangd_file("${PROJ_CXX_FLAGS}")
endif()

set(PATCHES_DIR "${CMAKE_CURRENT_LIST_DIR}/patches")

add_subdirectory(mEn)
add_subdirectory(kEn)

add_library(mEn::mEn ALIAS mEn)
add_library(kEn::kEn ALIAS kEn)

if (BUILD_SANDBOX)
  add_subdirectory(Sandbox)
endif()
