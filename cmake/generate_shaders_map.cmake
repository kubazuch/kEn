if(NOT DEFINED OUTPUT_FILE)
  message(FATAL_ERROR "OUTPUT_FILE not defined")
endif()
if(NOT DEFINED HEADER_FILES)
  message(FATAL_ERROR "HEADER_FILES not defined")
endif()
if(NOT DEFINED SHADER_BASE_NAMES)
  message(FATAL_ERROR "SHADER_BASE_NAMES not defined")
endif()
if(NOT DEFINED SHADER_VAR_NAMES)
  message(FATAL_ERROR "SHADER_VAR_NAMES not defined")
endif()

# Begin constructing the mapping header.
set(content "#pragma once\n\n")
set(content "${content}#include <string_view>\n")
set(content "${content}#include <unordered_map>\n\n")

separate_arguments(SHADER_BASE_NAMES UNIX_COMMAND "${SHADER_BASE_NAMES}")
separate_arguments(SHADER_VAR_NAMES UNIX_COMMAND "${SHADER_VAR_NAMES}")
separate_arguments(HEADER_FILES UNIX_COMMAND "${HEADER_FILES}")

# Include all individual shader headers (assumed to be in the same generated directory)
foreach(header ${HEADER_FILES})
  get_filename_component(header_basename ${header} NAME)
  set(content "${content}#include \"${header_basename}\"\n")
endforeach()

set(content "${content}\n")
set(content "${content}#define GENERATED_SHADER_MAP  {\\\n")

# Loop over the lists (assumed to have the same length)
list(LENGTH SHADER_BASE_NAMES numShaders)
math(EXPR lastIndex "${numShaders} - 1")
foreach(i RANGE 0 ${lastIndex})
  list(GET SHADER_BASE_NAMES ${i} shaderKey)
  list(GET SHADER_VAR_NAMES ${i} shaderVar)
  set(content "${content}    {\"${shaderKey}\", ${shaderVar}},\\\n")
endforeach()

set(content "${content}};\\\n")

file(WRITE "${OUTPUT_FILE}" "${content}")
