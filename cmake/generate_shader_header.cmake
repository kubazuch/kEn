if(NOT DEFINED INPUT_FILE OR NOT DEFINED OUTPUT_FILE OR NOT DEFINED VARIABLE_NAME)
  message(FATAL_ERROR "INPUT_FILE, OUTPUT_FILE, and VARIABLE_NAME must be defined")
endif()

# Read the shader file content
file(READ "${INPUT_FILE}" file_content)

# Prepare header content using "EOF" as the delimiter.
set(header_content "#pragma once\n\n")
set(header_content "${header_content}#include <string_view>\n\n")
set(header_content "${header_content}constexpr std::string_view ${VARIABLE_NAME} = R\"EOF(\n")
set(header_content "${header_content}${file_content}\n)EOF\";\n")

# Write the header file
file(WRITE "${OUTPUT_FILE}" "${header_content}")
