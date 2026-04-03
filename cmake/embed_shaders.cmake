include_guard(GLOBAL)

function(embed_shaders)
  cmake_parse_arguments(ARGS "" "NAME;SHADER_DIR;GENERATED_DIR" "" ${ARGN})

  # Find all shader files recursively; re-run CMake configure when files change
  file(GLOB_RECURSE SHADER_FILES CONFIGURE_DEPENDS
    "${ARGS_SHADER_DIR}/*.glsl"
  )
  file(MAKE_DIRECTORY "${ARGS_GENERATED_DIR}")

  # Lists to store generated header paths and mapping info
  set(GENERATED_HEADERS "")
  set(SHADER_BASE_NAMES "")
  set(SHADER_VAR_NAMES "")

  foreach(shader_file ${SHADER_FILES})
    # Base name without extension used as the map key (e.g. "basic")
    get_filename_component(shader_name "${shader_file}" NAME_WE)

    # Sanitize to produce a valid C++ identifier
    string(REGEX REPLACE "[^A-Za-z0-9_]" "_" var_name "${shader_name}")
    set(var_name "${var_name}_shader")

    list(APPEND SHADER_BASE_NAMES "${shader_name}")
    list(APPEND SHADER_VAR_NAMES "${var_name}")

    set(output_header "${ARGS_GENERATED_DIR}/${shader_name}.hpp")

    add_custom_command(
        OUTPUT "${output_header}"
        COMMAND ${CMAKE_COMMAND} -E echo "Generating header for ${shader_file}"
        COMMAND ${CMAKE_COMMAND}
            "-DINPUT_FILE=${shader_file}"
            "-DOUTPUT_FILE=${output_header}"
            "-DVARIABLE_NAME=${var_name}"
            -P "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/generate_shader_header.cmake"
        DEPENDS "${shader_file}"
        COMMENT "Embedding shader ${shader_file}"
    )
    list(APPEND GENERATED_HEADERS "${output_header}")
  endforeach()

  # Target to generate all individual shader headers
  add_custom_target("${ARGS_NAME}__generate_shader_headers"
      DEPENDS ${GENERATED_HEADERS}
  )

  # Escape semicolons so the shell does not interpret them as command separators
  # when CMake expands these list variables inside the COMMAND arguments.
  string(REPLACE ";" "\\;" SHADER_BASE_NAMES_SAFE "${SHADER_BASE_NAMES}")
  string(REPLACE ";" "\\;" SHADER_VAR_NAMES_SAFE "${SHADER_VAR_NAMES}")
  string(REPLACE ";" "\\;" GENERATED_HEADERS_SAFE "${GENERATED_HEADERS}")

  # Target to generate the overall mapping header (shaders.hpp)
  set(SHADERS_MAP_OUTPUT "${ARGS_GENERATED_DIR}/shaders.hpp")
  add_custom_command(
      OUTPUT "${SHADERS_MAP_OUTPUT}"
      COMMAND ${CMAKE_COMMAND} -E echo "Generating shaders map header"
      COMMAND ${CMAKE_COMMAND}
         "-DSHADER_BASE_NAMES=${SHADER_BASE_NAMES_SAFE}"
         "-DSHADER_VAR_NAMES=${SHADER_VAR_NAMES_SAFE}"
         "-DHEADER_FILES=${GENERATED_HEADERS_SAFE}"
         "-DOUTPUT_FILE=${SHADERS_MAP_OUTPUT}"
         -P "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/generate_shaders_map.cmake"
      DEPENDS ${GENERATED_HEADERS}
      COMMENT "Creating shaders.hpp"
  )
  add_custom_target("${ARGS_NAME}__generate_shaders_map"
      DEPENDS "${SHADERS_MAP_OUTPUT}"
  )

  # Top-level target; this is the one callers should depend on
  add_custom_target("${ARGS_NAME}__embed_shaders" ALL
      DEPENDS "${ARGS_NAME}__generate_shader_headers" "${ARGS_NAME}__generate_shaders_map"
  )

  set(EMBEDDED_SHADERS_HEADER "${SHADERS_MAP_OUTPUT}" PARENT_SCOPE)
  set(EMBED_SHADERS_GENERATED_DIR "${ARGS_GENERATED_DIR}" PARENT_SCOPE)
  set(EMBED_SHADERS_TARGET "${ARGS_NAME}__embed_shaders" PARENT_SCOPE)
endfunction()
