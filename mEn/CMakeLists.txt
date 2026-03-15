cmake_minimum_required(VERSION 3.20)

# Allow mEn to be used either as a standalone project or as a subdirectory.
if(NOT DEFINED PROJECT_NAME)
  project(mEn)
endif()

message(STATUS "Configuring mEn")

# Option to switch between GLM and native implementations
option(MEN_USE_GLM "Use GLM library instead of internal implementations" OFF)
option(MEN_BUILD_GLM "Fetch and build GLM. When OFF, CMake will look for proper GLM version on your operating system." ON)

include(FetchContent)

add_library(mEn INTERFACE)

# Prevent CMake from adding `lib` before `mEn`
set_target_properties(mEn PROPERTIES PREFIX "")
set_target_properties(mEn PROPERTIES LINKER_LANGUAGE CXX)

target_compile_features(mEn INTERFACE cxx_std_23)
target_include_directories(mEn
        INTERFACE src "${CMAKE_BINARY_DIR}/generated")

if(MEN_USE_GLM OR BUILD_TESTING)
    if(NOT MEN_BUILD_GLM)
        find_package(GLM 1.0.3)
    else()
        message(CHECK_START "Fetching GLM")
        list(APPEND CMAKE_MESSAGE_INDENT "  ")

        FetchContent_Declare(
        glm
        GIT_REPOSITORY "https://github.com/g-truc/glm.git"
        GIT_TAG "1.0.3"
        GIT_SHALLOW TRUE
        PATCH_COMMAND git apply --ignore-whitespace
                        "${CMAKE_CURRENT_LIST_DIR}/../patches/glm.patch"
        UPDATE_DISCONNECTED 1)
        FetchContent_MakeAvailable(glm)

        list(POP_BACK CMAKE_MESSAGE_INDENT)
        message(CHECK_PASS "fetched")
    endif()
endif()

if(MEN_USE_GLM)
    target_compile_definitions(mEn INTERFACE MEN_GLM GLM_ENABLE_EXPERIMENTAL)
    target_link_libraries(mEn INTERFACE $<BUILD_INTERFACE:glm::glm>)
    target_include_directories(mEn SYSTEM INTERFACE
        $<TARGET_PROPERTY:glm::glm,INTERFACE_INCLUDE_DIRECTORIES>)
endif()

if(BUILD_TESTING)
    enable_testing()

    # NOTE: Adding new .cpp files requires re-running CMake (GLOB_RECURSE does not
    # trigger automatic reconfiguration).
    file(GLOB_RECURSE TESTS_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp")

    add_executable(mEn_tests ${TESTS_SOURCES})
    target_compile_definitions(mEn_tests PRIVATE MEN_GLM GLM_ENABLE_EXPERIMENTAL)
    target_link_libraries(mEn_tests PRIVATE mEn GTest::gtest_main)
    target_include_directories(mEn_tests SYSTEM PRIVATE
        $<TARGET_PROPERTY:glm::glm,INTERFACE_INCLUDE_DIRECTORIES>)

    include(GoogleTest)
    gtest_discover_tests(mEn_tests TEST_PREFIX "mEn.")
endif()
