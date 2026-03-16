include_guard(GLOBAL)

function(configure_library)
  # Parse arguments
  set(options HEADER_ONLY)
  set(oneValueArgs NAME)
  set(multiValueArgs DEPS_PUBLIC INCLUDE_DIRS COMPILE_DEFINITIONS)
  cmake_parse_arguments(ARGS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  cmake_minimum_required(VERSION 3.28 FATAL_ERROR)

  set(CMAKE_CXX_STANDARD 23)

  message(STATUS "Configuring " ${PROJECT_NAME})
  list(APPEND CMAKE_MESSAGE_INDENT "  [${PROJECT_NAME}] ")

  if(ARGS_HEADER_ONLY)
    add_library(${PROJECT_NAME} INTERFACE)
    
    if(USE_SYSTEM_INCLUDE)
      target_include_directories(${PROJECT_NAME} SYSTEM INTERFACE 
        src 
        "${CMAKE_BINARY_DIR}/generated"
        "${ARGS_INCLUDE_DIRS}"
      )
    else()
      target_include_directories(${PROJECT_NAME} INTERFACE 
        src 
        "${CMAKE_BINARY_DIR}/generated"
        "${ARGS_INCLUDE_DIRS}"
      )
    endif()

    # Dependencies
    set(DEPS_PUBLIC ${ARGS_DEPS_PUBLIC})
    if(${TRACY_ENABLE})
        list(APPEND DEPS_PUBLIC TracyClient)
    endif()
    message(STATUS "Requested libraries: ${DEPS_PUBLIC}")
    target_link_libraries(${PROJECT_NAME} INTERFACE ${DEPS_PUBLIC})

    # Compile definitions
    if(ARGS_COMPILE_DEFINITIONS)
      message(STATUS "Default compile definitions: ")
      message(STATUS "Requested compile definitions: ${ARGS_COMPILE_DEFINITIONS}")
      target_compile_definitions(${PROJECT_NAME} INTERFACE 
        ${ARGS_COMPILE_DEFINITIONS}
      )
    else()
      message(STATUS "Default compile definitions: ")
      target_compile_definitions(${PROJECT_NAME} INTERFACE 
      )
    endif()
  else()
    file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

    if (NOT SOURCES)
        message(FATAL_ERROR "No source files detected for target: ${PROJECT_NAME}. If it's header-only library, consider using HEADER_ONLY option")
    endif()

    add_library(${PROJECT_NAME} ${SOURCES})
    
    if(USE_SYSTEM_INCLUDE)
      message("SYSTEM PUBLIC")
      target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC 
        src 
        "${CMAKE_BINARY_DIR}/generated"
        "${ARGS_INCLUDE_DIRS}"
      )
    else()
      message("PUBLIC")
      target_include_directories(${PROJECT_NAME} PUBLIC 
        src
        "${CMAKE_BINARY_DIR}/generated"
        "${ARGS_INCLUDE_DIRS}"
      )
    endif()

    # Dependencies
    set(DEPS_PUBLIC ${ARGS_DEPS_PUBLIC})
    if(${TRACY_ENABLE})
      list(APPEND DEPS_PUBLIC TracyClient)
    endif()
    message(STATUS "Dependencies: ${DEPS_PUBLIC}")
    if (ARGS_INCLUDE_DIRS)
      message(STATUS "Requested include directories: ${ARGS_INCLUDE_DIRS}")
    endif()
    target_link_libraries(${PROJECT_NAME} PUBLIC ${DEPS_PUBLIC})

    # Compile definitions
    message(STATUS "Default compile definitions: ")
    target_compile_definitions(${PROJECT_NAME} PRIVATE 
    )
    if(ARGS_COMPILE_DEFINITIONS)
        message(STATUS "Requested compile definitions: ${ARGS_COMPILE_DEFINITIONS}")
        target_compile_definitions(${PROJECT_NAME} PUBLIC ${ARGS_COMPILE_DEFINITIONS})
    endif()

    # Compiler and linker
    target_compile_options(${PROJECT_NAME} PRIVATE ${PROJ_CXX_FLAGS})
    target_link_options(${PROJECT_NAME} PRIVATE ${PROJ_SHARED_LINKER_FLAGS})
  endif()

  # Tests configuration
  if(BUILD_TESTS)
    # Include CTest in order to generate `DartConfiguration.tcl`
    include(CTest)

    message(STATUS "Configuring ${PROJECT_NAME}_tests executable")
    file(GLOB_RECURSE TESTS_SOURCES 
        "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp"
    )
    if(NOT TESTS_SOURCES)
      message(STATUS "No tests sources found, tests executable configuration stopped")
    else()
      enable_testing()

      add_executable(
          "${PROJECT_NAME}_tests"
          ${TESTS_SOURCES} 
      )
      target_link_libraries(
          "${PROJECT_NAME}_tests"
          GTest::gtest_main
          ${PROJECT_NAME}
      )

      include(GoogleTest)
      gtest_discover_tests("${PROJECT_NAME}_tests" TEST_PREFIX "${PROJECT_NAME}.")
    endif()
  endif()

  list(POP_BACK CMAKE_MESSAGE_INDENT)
endfunction()