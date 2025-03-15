set(CURRENT_DIR ${CMAKE_CURRENT_LIST_DIR})  
function(embed_shaders SHADER_DIR GENERATED_SHADERS_DIR)
  # Find all shader files in SHADER_DIR
  file(GLOB SHADER_FILES "${SHADER_DIR}/*")
  file(MAKE_DIRECTORY "${GENERATED_SHADERS_DIR}")

  # Lists to store generated header paths and mapping info
  set(GENERATED_HEADERS "")
  set(SHADER_BASE_NAMES "")
  set(SHADER_VAR_NAMES "")

  foreach(shader_file ${SHADER_FILES})
    # Get the base name without extension (e.g. "basic.vert" becomes "basic")
    get_filename_component(shader_name ${shader_file} NAME_WE)
    
    # Sanitize the shader name to create a valid variable name (replace non-alphanumerics with underscore)
    string(REGEX REPLACE "[^A-Za-z0-9_]" "_" var_name ${shader_name})
    set(var_name "${var_name}_shader")

    list(APPEND SHADER_BASE_NAMES ${shader_name})
    list(APPEND SHADER_VAR_NAMES ${var_name})
    # Define the output header for this shader
    set(output_header "${GENERATED_SHADERS_DIR}/${shader_name}.hpp")

    add_custom_command(
        OUTPUT ${output_header}
        COMMAND ${CMAKE_COMMAND} -E echo "Generating header for ${shader_file}"
        COMMAND ${CMAKE_COMMAND}
            -DINPUT_FILE="${shader_file}"
            -DOUTPUT_FILE="${output_header}"
            -DVARIABLE_NAME="${var_name}"
            -P "${CURRENT_DIR}/generate_shader_header.cmake"
        DEPENDS ${shader_file}
        COMMENT "Embedding shader ${shader_file}"
    )
    list(APPEND GENERATED_HEADERS ${output_header})
  endforeach()

  # Create a target to generate all individual shader headers.
  add_custom_target(GenerateShadersHeaders ALL DEPENDS ${GENERATED_HEADERS})

  # Now generate the overall mapping header (shaders.hpp).
  set(SHADERS_MAP_OUTPUT "${GENERATED_SHADERS_DIR}/shaders.hpp")
  add_custom_command(
      OUTPUT ${SHADERS_MAP_OUTPUT}
      COMMAND ${CMAKE_COMMAND} -E echo "Generating shaders map header"
      COMMAND ${CMAKE_COMMAND}
         -DSHADER_BASE_NAMES="${SHADER_BASE_NAMES}"
         -DSHADER_VAR_NAMES="${SHADER_VAR_NAMES}"
         -DHEADER_FILES="${GENERATED_HEADERS}"
         -DOUTPUT_FILE="${SHADERS_MAP_OUTPUT}"
         -P "${CURRENT_DIR}/generate_shaders_map.cmake"
      DEPENDS ${GENERATED_HEADERS}
      COMMENT "Creating shaders.hpp"
  )
  add_custom_target(GenerateShadersMap ALL DEPENDS ${SHADERS_MAP_OUTPUT})

  # Create an overall target that depends on both individual and map headers.
  add_custom_target(EmbedShaders ALL DEPENDS GenerateShadersHeaders GenerateShadersMap)

  # Optionally, export the generated mapping header and the generated directory to the caller.
  set(EMBEDDED_SHADERS_HEADER "${SHADERS_MAP_OUTPUT}" PARENT_SCOPE)
  set(EMBED_SHADERS_GENERATED_DIR "${GENERATED_SHADERS_DIR}" PARENT_SCOPE)
endfunction()