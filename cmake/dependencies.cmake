include_guard(GLOBAL)
include(FetchContent)

if(NOT PATCHES_DIR)
  message(FATAL_ERROR "No patches dir set")
endif()

find_package(Git REQUIRED)

if(NOT IS_DIRECTORY "${PATCHES_DIR}")
  message(FATAL_ERROR "PATCHES_DIR does not exist: ${PATCHES_DIR}")
endif()

macro(_fetch_begin name)
  message(CHECK_START "Fetching ${name}")
  list(APPEND CMAKE_MESSAGE_INDENT "  ")
  set(CMAKE_EXPORT_COMPILE_COMMANDS OFF)
  set(BUILD_SHARED_LIBS OFF)
endmacro()

macro(_fetch_end)
  if(_ken_dep_name)
    _silence_dep_warnings("${${_ken_dep_name}_SOURCE_DIR}" "${_ken_dep_subdir}")
  endif()
  list(POP_BACK CMAKE_MESSAGE_INDENT)
  message(CHECK_PASS "fetched")
endmacro()

# Compiles a dependency's own sources with warnings disabled; they are not ours to fix, and its headers are already
# SYSTEM for our targets. Walks every directory the dependency added.
function(_silence_dep_warnings source_dir subdir)
  if(subdir)
    string(APPEND source_dir "/${subdir}")
  endif()
  if(MSVC)
    set(_no_warnings /w)
  else()
    set(_no_warnings -w)
  endif()

  set(_dirs "${source_dir}")
  while(_dirs)
    list(POP_FRONT _dirs _dir)
    get_property(_targets DIRECTORY "${_dir}" PROPERTY BUILDSYSTEM_TARGETS)
    foreach(_target IN LISTS _targets)
      get_target_property(_type ${_target} TYPE)
      if(_type MATCHES "^(STATIC_LIBRARY|SHARED_LIBRARY|MODULE_LIBRARY|OBJECT_LIBRARY|EXECUTABLE)$")
        target_compile_options(${_target} PRIVATE ${_no_warnings})
      endif()
    endforeach()
    get_property(_subdirs DIRECTORY "${_dir}" PROPERTY SUBDIRECTORIES)
    list(APPEND _dirs ${_subdirs})
  endwhile()
endfunction()

set(KEN_DEPS_SOURCE_DIR "" CACHE PATH
  "If set, dependency sources are fetched (and patched) once into this directory and shared by every build tree using it")

# Fetches and patches a dependency into KEN_DEPS_SOURCE_DIR unless already there, returning its directory in out_var.
# The directory is keyed by tag and patch so a version bump or patch edit never reuses stale sources.
function(_populate_shared name tag patch_file out_var)
  set(_key "${tag}")
  if(patch_file)
    file(SHA1 "${patch_file}" _patch_hash)
    string(SUBSTRING "${_patch_hash}" 0 8 _patch_hash)
    string(APPEND _key "-${_patch_hash}")
  endif()

  set(_dir "${KEN_DEPS_SOURCE_DIR}/${name}-${_key}")
  set(_marker "${_dir}/.ken-populated")
  set(${out_var} "${_dir}" PARENT_SCOPE)

  if(EXISTS "${_marker}")
    return()
  endif()

  # serializes build trees configuring at the same time; released when the function returns
  file(MAKE_DIRECTORY "${KEN_DEPS_SOURCE_DIR}")
  file(LOCK "${_dir}.lock" GUARD FUNCTION TIMEOUT 1800)
  if(EXISTS "${_marker}")
    return()
  endif()

  file(REMOVE_RECURSE "${_dir}")  # leftovers of an interrupted population
  FetchContent_Populate(
    ken_shared_${name}
    QUIET
    SOURCE_DIR   "${_dir}"
    BINARY_DIR   "${CMAKE_BINARY_DIR}/_deps/ken_shared_${name}-build"
    SUBBUILD_DIR "${CMAKE_BINARY_DIR}/_deps/ken_shared_${name}-subbuild"
    ${ARGN}
  )
  file(TOUCH "${_marker}")
endfunction()

function(_declare_dep name repo tag)
  set(options NO_SHALLOW)
  set(oneValueArgs PATCH_FILE SOURCE_SUBDIR)
  cmake_parse_arguments(DEP "${options}" "${oneValueArgs}" "" ${ARGN})

  if(DEP_NO_SHALLOW)
    set(_shallow FALSE)
  else()
    set(_shallow TRUE)
  endif()

  set(_fetch_args
    GIT_REPOSITORY "${repo}"
    GIT_TAG "${tag}"
    GIT_SHALLOW ${_shallow}
    UPDATE_DISCONNECTED ON
  )

  if(DEP_PATCH_FILE)
    list(APPEND _fetch_args
      PATCH_COMMAND "${GIT_EXECUTABLE}" apply --ignore-whitespace "${DEP_PATCH_FILE}"
    )
  endif()

  if(KEN_DEPS_SOURCE_DIR)
    _populate_shared(${name} "${tag}" "${DEP_PATCH_FILE}" _shared_dir ${_fetch_args})
    # makes FetchContent use the shared sources as they are, skipping download and patch
    string(TOUPPER "${name}" _upper_name)
    set(FETCHCONTENT_SOURCE_DIR_${_upper_name} "${_shared_dir}" PARENT_SCOPE)
  endif()

  set(_args ${_fetch_args} SYSTEM)

  if(DEP_SOURCE_SUBDIR)
    list(APPEND _args
      SOURCE_SUBDIR "${DEP_SOURCE_SUBDIR}"
    )
  endif()

  FetchContent_Declare(${name} ${_args})

  # for _fetch_end, once the dependency is made available
  set(_ken_dep_name "${name}" PARENT_SCOPE)
  set(_ken_dep_subdir "${DEP_SOURCE_SUBDIR}" PARENT_SCOPE)
endfunction()

# ##############################################################################
# GLAD
# ##############################################################################
function(fetch_glad)
  if(TARGET glad)
    return()
  endif()

  block()
    _fetch_begin("GLAD")

    _declare_dep(
      glad
      "https://github.com/Dav1dde/glad.git"
      "v2.0.8"
      SOURCE_SUBDIR cmake
    )

    FetchContent_MakeAvailable(glad)

    glad_add_library(
      glad
      STATIC
      REPRODUCIBLE
      EXCLUDE_FROM_ALL
      LOADER
      API
      gl:core=4.6)

    set_property(TARGET glad PROPERTY SYSTEM ON)

    _fetch_end()
  endblock()
endfunction()

# ##############################################################################
# GLFW
# ##############################################################################
function(fetch_glfw)
  if(TARGET glfw)
    return()
  endif()

  block()
    _fetch_begin("GLFW")

    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Do not build GLFW example programs" FORCE)
    set(GLFW_BUILD_TESTS    OFF CACHE BOOL "Do not build GLFW test programs"    FORCE)
    set(GLFW_BUILD_DOCS     OFF CACHE BOOL "Do not build GLFW documentation"    FORCE)
    set(GLFW_INSTALL        OFF CACHE BOOL "Do not install the GLFW"            FORCE)

    _declare_dep(
      glfw
      "https://github.com/glfw/glfw.git"
      "3.4"
    )

    FetchContent_MakeAvailable(glfw)

    _fetch_end()
  endblock()
endfunction()

# ##############################################################################
# ImGui (depends on GLFW)
# ##############################################################################
function(fetch_imgui)
  if(TARGET imgui)
    return()
  endif()

  block()
    _fetch_begin("ImGui")

    _declare_dep(
      imgui
      "https://github.com/ocornut/imgui.git"
      "v1.92.6-docking"
      PATCH_FILE "${PATCHES_DIR}/imgui.patch"
    )

    FetchContent_MakeAvailable(imgui)

    _fetch_end()
  endblock()
endfunction()

# ##############################################################################
# ImGuizmo (depends on ImGui)
# ##############################################################################
function(fetch_imguizmo)
  if(TARGET ImGuizmo)
    return()
  endif()

  block()
    _fetch_begin("ImGuizmo")

    _declare_dep(
      imguizmo
      "https://github.com/CedricGuillemet/ImGuizmo.git"
      "1.83"
      PATCH_FILE "${PATCHES_DIR}/imguizmo.patch"
    )

    FetchContent_MakeAvailable(imguizmo)

    _fetch_end()
  endblock()
endfunction()

# ##############################################################################
# nlohmann/json
# ##############################################################################
function(fetch_nlohmann_json)
  if(TARGET nlohmann_json::nlohmann_json)
    return()
  endif()

  block()
    _fetch_begin("nlohmann/json")

    _declare_dep(
      nlohmann_json
      "https://github.com/nlohmann/json.git"
      "v3.12.0"
    )

    FetchContent_MakeAvailable(nlohmann_json)

    _fetch_end()
  endblock()
endfunction()

# ##############################################################################
# assimp
# ##############################################################################
function(fetch_assimp)
  if(TARGET assimp)
    return()
  endif()

  block()
    _fetch_begin("assimp")

    set(ASSIMP_BUILD_TESTS                    OFF CACHE BOOL "Do not build assimp tests"          FORCE)
    set(ASSIMP_INSTALL                        OFF CACHE BOOL "Do not install assimp"              FORCE)
    set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE BOOL "Disable all importers"              FORCE)
    set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF CACHE BOOL "Disable all exporters"              FORCE)
    set(ASSIMP_BUILD_OBJ_IMPORTER             ON  CACHE BOOL "Enable OBJ importer"                FORCE)
    set(ASSIMP_BUILD_FBX_IMPORTER             ON  CACHE BOOL "Enable FBX importer"                FORCE)
    set(ASSIMP_BUILD_ASSIMP_VIEW              OFF CACHE BOOL "Do not build the assimp view tool." FORCE)
    set(ASSIMP_BUILD_ZLIB                     ON  CACHE BOOL "Build the zlib"                     FORCE)
    set(ASSIMP_WARNINGS_AS_ERRORS             OFF CACHE BOOL "Third-party warnings must not fail the build" FORCE)

    _declare_dep(
      assimp
      "https://github.com/assimp/assimp.git"
      "v6.0.4"
      PATCH_FILE "${PATCHES_DIR}/assimp.patch"
    )

    FetchContent_MakeAvailable(assimp)

    _fetch_end()
  endblock()
endfunction()

# ##############################################################################
# nativefiledialog-extended
# ##############################################################################
function(fetch_nfd)
  if(TARGET nfd)
    return()
  endif()

  block()
    _fetch_begin("nativefiledialog-extended")

    # IMPORTANT: The xdg-desktop-portal allows to display the actual native file dialog
    # on linux systems (by default the nfd uses gtk). However there might be some issues
    # on DEs that are not directly supporting the portals. The rule of thumb is that everywhere
    # you can run file dialog from your flatpak app, the portals should work seamlessly.
    set(NFD_PORTAL ON CACHE BOOL "Use portal to use native file dialog instead of the gtk" FORCE)

    _declare_dep(
      nfd
      "https://github.com/btzy/nativefiledialog-extended.git"
      "v1.3.0"
      PATCH_FILE "${PATCHES_DIR}/nfd.patch"
    )

    FetchContent_MakeAvailable(nfd)

    _fetch_end()
  endblock()
endfunction()

# ##############################################################################
# spdlog
# ##############################################################################
function(fetch_spdlog)
  if(TARGET spdlog)
    return()
  endif()

  block()
    _fetch_begin("spdlog")

    set(SPDLOG_USE_STD_FORMAT  ON CACHE BOOL "Use C++20 std::format instead of fmt::format" FORCE)

    _declare_dep(
      spdlog
      "https://github.com/gabime/spdlog.git"
      "v1.17.0"
    )

    FetchContent_MakeAvailable(spdlog)

    _fetch_end()
  endblock()
endfunction()

# ##############################################################################
# stb
# ##############################################################################
function(fetch_stb)
  if(TARGET stb)
    return()
  endif()

  block()
    _fetch_begin("stb")

    _declare_dep(
      stb
      "https://github.com/nothings/stb.git"
      "904aa67e1e2d1dec92959df63e700b166d5c1022"
      NO_SHALLOW
      PATCH_FILE "${PATCHES_DIR}/stb.patch"
    )

    FetchContent_MakeAvailable(stb)

    _fetch_end()
  endblock()
endfunction()

# ##############################################################################
# GLM
# ##############################################################################
function(fetch_glm)
  if(TARGET glm)
    return()
  endif()

  block()
    _fetch_begin("GLM")

    set(GLM_BUILD_LIBRARY OFF CACHE BOOL "Do not build GLM as a static library" FORCE)

    _declare_dep(
      glm
      "https://github.com/g-truc/glm.git"
      "1.0.3"
    )

    FetchContent_MakeAvailable(glm)

    _fetch_end()
  endblock()
endfunction()

# ##############################################################################
# json-schema-validator
# ##############################################################################
function(fetch_json_schema_validator)
  if(TARGET nlohmann_json_schema_validator)
    return()
  endif()

  block()
    _fetch_begin("json-schema-validator")

    set(JSON_VALIDATOR_INSTALL OFF CACHE BOOL "Do not install json-schema-validator" FORCE)

    _declare_dep(
      json_schema_validator
      "https://github.com/pboettch/json-schema-validator.git"
      "2.3.0"
    )

    FetchContent_MakeAvailable(json_schema_validator)

    _fetch_end()
  endblock()
endfunction()

# ##############################################################################
# Tracy
# ##############################################################################
function(fetch_tracy)
  if(TARGET TracyClient)
    return()
  endif()

  block()
    _fetch_begin("Tracy")

    set(TRACY_ON_DEMAND ON CACHE BOOL "Collect profiling data only while the profiler is connected" FORCE)

    _declare_dep(
      tracy
      "https://github.com/wolfpld/tracy.git"
      "v0.14.1"
    )

    FetchContent_MakeAvailable(tracy)

    _fetch_end()
  endblock()
endfunction()

# ##############################################################################
# GTest
# ##############################################################################
function(fetch_googletest)
  if(TARGET googletest)
    return()
  endif()

  block()
    _fetch_begin("googletest")

    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    _declare_dep(
      googletest
      "https://github.com/google/googletest.git"
      "v1.17.0"
    )

    FetchContent_MakeAvailable(googletest)

    _fetch_end()
  endblock()
endfunction()
