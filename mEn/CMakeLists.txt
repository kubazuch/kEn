cmake_minimum_required(VERSION 3.20)

project(mEn)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
message(STATUS "Configuring " ${PROJECT_NAME})

# Option to switch between GLM and native implementations
option(MEN_GLM "Use GLM library instead of internal implementations" OFF)
option(MEN_BUILD_GLM "Fetch and build GLM. When OFF, CMake will look for proper GLM version on your operating system." ON)

set(CMAKE_CXX_STANDARD 23) 

add_library(${PROJECT_NAME} INTERFACE)

# Prevent CMake from adding `lib` before `mEn`
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(${PROJECT_NAME}
        INTERFACE src "${CMAKE_BINARY_DIR}/generated")

if(MEN_GLM)
        if(NOT MEN_BUILD_GLM)
                find_package(GLM 1.0.1)
        else()
                message(CHECK_START "Fetching GLM")
                list(APPEND CMAKE_MESSAGE_INDENT "  ")

                FetchContent_Declare(
                glm
                GIT_REPOSITORY "https://github.com/g-truc/glm.git"
                GIT_TAG "1.0.1"
                PATCH_COMMAND git apply --ignore-whitespace
                                "${CMAKE_CURRENT_LIST_DIR}/../patches/glm.patch"
                UPDATE_DISCONNECTED 1)
                FetchContent_MakeAvailable(glm)

                list(POP_BACK CMAKE_MESSAGE_INDENT)
                message(CHECK_PASS "fetched")
        endif()

        target_compile_definitions(${PROJECT_NAME} INTERFACE MEN_GLM GLM_ENABLE_EXPERIMENTAL)
        target_link_libraries(${PROJECT_NAME} INTERFACE glm::glm)
endif()