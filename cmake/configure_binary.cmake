include_guard(GLOBAL)

function(configure_binary)
  # Parse arguments
  set(options HEADER_ONLY)
  set(oneValueArgs NAME)
  set(multiValueArgs DEPS_PUBLIC INCLUDE_DIRS COMPILE_DEFINITIONS SHADER_TARGETS)
  cmake_parse_arguments(ARGS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  cmake_minimum_required(VERSION 3.28)

  project(${ARGS_NAME} CXX)
  message(STATUS "Configuring " ${PROJECT_NAME})

  set(CMAKE_CXX_STANDARD 23)

  message(STATUS "Configuring binary ${PROJECT_NAME}")
  list(APPEND CMAKE_MESSAGE_INDENT "  [${PROJECT_NAME}] ")

  # Executable target configuration
  file(GLOB_RECURSE SOURCES 
      "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
  )
  add_executable(${PROJECT_NAME} ${SOURCES})
  target_link_libraries(${PROJECT_NAME} PUBLIC ${ARGS_DEPS_PUBLIC})
  target_include_directories(${PROJECT_NAME} PUBLIC src "${CMAKE_BINARY_DIR}/generated" "${ARGS_INCLUDE_DIRS}")
  target_compile_options(${PROJECT_NAME} PRIVATE ${PROJ_CXX_FLAGS})
  if(BUILD_SHARED_LIBS AND PROJ_EXE_LINKER_FLAGS)
      target_link_options(${PROJECT_NAME} PRIVATE ${PROJ_EXE_LINKER_FLAGS})
  endif()
  if(ARGS_COMPILE_DEFINITIONS)
      message(STATUS "Default compile definitions: ")
      message(STATUS "Requested compile definitions: ${ARGS_COMPILE_DEFINITIONS}")
      target_compile_definitions(${PROJECT_NAME} PRIVATE
          ${ARGS_COMPILE_DEFINITIONS}
      )
  else()
      message(STATUS "Default compile definitions: ")
      target_compile_definitions(${PROJECT_NAME} PRIVATE
      )
  endif()

  if (ARGS_SHADER_TARGETS)
      add_dependencies(${PROJECT_NAME} ${ARGS_SHADER_TARGETS})
  endif()

  # Copy assets folder to build directory
  if(EXISTS "${PROJECT_SOURCE_DIR}/assets")
      add_custom_target("${PROJECT_NAME}__copy_assets"
      COMMAND ${CMAKE_COMMAND} -DASSETS_SOURCE_DIR=${PROJECT_SOURCE_DIR}/assets
                              -DASSETS_DEST_DIR=$<TARGET_FILE_DIR:${PROJECT_NAME}>
                              -P ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/copy_assets.cmake
      )
      add_dependencies(${PROJECT_NAME} "${PROJECT_NAME}__copy_assets")
  endif()

  if(ARGS_SHADER_TARGETS AND EXISTS "${PROJECT_SOURCE_DIR}/shaders")
    add_custom_target("${PROJECT_NAME}__copy_shaders"
        COMMAND ${CMAKE_COMMAND} -DASSETS_SOURCE_DIR=${PROJECT_SOURCE_DIR}/shaders
                            -DASSETS_DEST_DIR=$<TARGET_FILE_DIR:${PROJECT_NAME}>
                            -P ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/copy_assets.cmake
    )
    add_dependencies("${PROJECT_NAME}__copy_shaders" ${ARGS_SHADER_TARGETS})
    add_dependencies(${PROJECT_NAME} "${PROJECT_NAME}__copy_shaders")
  endif()


  list(POP_BACK CMAKE_MESSAGE_INDENT)
endfunction()