project(mEn CXX C)

include_guard()
include(../cmake/dependencies.cmake)
include(../cmake/configure_library.cmake)

option(MEN_USE_GLM "Use GLM library and provide interoperability" OFF)
option(BUILD_TESTING "Fetch GoogleTest and build tests" ON)
option(USE_SYSTEM_INCLUDE "Marks includes as system" ON)

if(MEN_USE_GLM OR BUILD_TESTING)
  fetch_glm()
endif()

if (MEN_USE_GLM)
  set(GLM_DEPS DEPS_PUBLIC glm COMPILE_DEFINITIONS MEN_GLM GLM_ENABLE_EXPERIMENTAL)
endif()
configure_library(NAME mEn HEADER_ONLY ${GLM_DEPS})
