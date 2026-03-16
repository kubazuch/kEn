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
endmacro()

macro(_fetch_end)
  list(POP_BACK CMAKE_MESSAGE_INDENT)
  message(CHECK_PASS "fetched")
endmacro()

function(_declare_dep name repo tag)
  set(options)
  set(oneValueArgs PATCH_FILE)
  cmake_parse_arguments(DEP "${options}" "${oneValueArgs}" "" ${ARGN})

  if(DEP_PATCH_FILE)
    FetchContent_Declare(
      ${name}
      GIT_REPOSITORY "${repo}"
      GIT_TAG "${tag}"
      GIT_SHALLOW TRUE
      UPDATE_DISCONNECTED ON
      PATCH_COMMAND "${GIT_EXECUTABLE}" apply --ignore-whitespace "${DEP_PATCH_FILE}"
    )
  else()
    FetchContent_Declare(
      ${name}
      GIT_REPOSITORY "${repo}"
      GIT_TAG "${tag}"
      GIT_SHALLOW TRUE
      UPDATE_DISCONNECTED ON
    )
  endif()
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
      PATCH_FILE "${PATCHES_DIR}/glad.patch"
    )

    FetchContent_MakeAvailable(glad)
    FetchContent_GetProperties(glad)

    add_subdirectory("${glad_SOURCE_DIR}/cmake" glad_cmake)
    glad_add_library(
      glad
      STATIC
      REPRODUCIBLE
      EXCLUDE_FROM_ALL
      LOADER
      API
      gl:core=4.6)

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
      PATCH_FILE "${PATCHES_DIR}/glfw.patch"
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

    _declare_dep(
      assimp
      "https://github.com/assimp/assimp.git"
      "v6.0.4"
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
      PATCH_FILE "${PATCHES_DIR}/glm.patch"
    )

    FetchContent_MakeAvailable(glm)

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
