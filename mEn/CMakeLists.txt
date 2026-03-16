project(mEn CXX C)

include_guard()
include(../cmake/dependencies.cmake)
include(../cmake/configure_library.cmake)

option(MEN_USE_GLM "Use GLM library and provide interoperability" OFF)
option(BUILD_TESTS "Fetch GoogleTest and build tests" OFF)
option(USE_SYSTEM_INCLUDE "Marks includes as system" ON)

if(MEN_USE_GLM OR BUILD_TESTS)
  fetch_glm()
  set(GLM_DEPS DEPS_PUBLIC glm COMPILE_DEFINITIONS MEN_GLM GLM_ENABLE_EXPERIMENTAL)
endif()

if(BUILD_TESTS)
  fetch_googletest()
endif()

configure_library(NAME mEn HEADER_ONLY ${GLM_DEPS})
