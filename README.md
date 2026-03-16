# kEn

A 3D game engine written in C++23 for self-education, featuring a custom math library, OpenGL 4.6 renderer, ECS-like scene graph, and an event system.

Greatly influenced by [TheCherno](https://github.com/TheCherno)'s [Game Engine](https://www.youtube.com/playlist?list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT) series and [Hazel](https://github.com/TheCherno/Hazel), as well as my previous [Java project](https://github.com/kubazuch/KuZuEngine-old).

The project structure originates from [resin](https://github.com/gizmokis/resin), a project I contributed to, and was further refined following [eray](https://github.com/migoox/eray), which also used resin as its base.

## Modules

| Module | Description |
| ------ | ----------- |
| **mEn** | Header-only C++23 math library -- vectors, matrices, quaternions, GLSL-style free functions |
| **kEn** | 3D rendering/engine library -- OpenGL backend, layer stack, event system, scene graph |
| **Sandbox** | Demo application that consumes kEn |

## Requirements

- CMake 3.20+
- C++23 compiler: MSVC (VS 2022) or GCC 14 / Clang
- Ninja (recommended for dev builds)

All other dependencies are fetched automatically via CMake's `FetchContent`:
[GLAD](https://github.com/Dav1dde/glad) (OpenGL 4.6), [GLFW](https://github.com/glfw/glfw) 3.4, [ImGui](https://github.com/ocornut/imgui) 1.91.8, [ImGuizmo](https://github.com/CedricGuillemet/ImGuizmo) 1.83, [assimp](https://github.com/assimp/assimp) 5.4.3, [nlohmann/json](https://github.com/nlohmann/json) 3.11.3, [nativefiledialog-extended](https://github.com/btzy/nativefiledialog-extended) 1.2.1, [spdlog](https://github.com/gabime/spdlog) 1.15.1, [stb](https://github.com/nothings/stb)

## Building

The project uses [CMake Presets](https://cmake.org/cmake/help/latest/manual/cmake-presets.7.html).

### Windows (MSVC + Ninja)

```bash
# Debug
cmake --preset=dev-debug-windows-x64
cmake --build --preset=dev-debug-windows-x64

# Release
cmake --preset=release-windows-x64
cmake --build --preset=release-windows-x64
```

### Linux (GCC 14)

```bash
# Debug
cmake --preset=dev-debug-linux
cmake --build --preset=dev-debug-linux

# Release
cmake --preset=release-linux
cmake --build --preset=release-linux
```

Output: `build/bin/` (executables), `build/lib/` (libraries).

### CMake options

| Option | Default | Description |
|--------|---------|-------------|
| `BUILD_TESTING` | `OFF` | Build mEn unit tests (GoogleTest) |
| `MEN_USE_GLM` | `OFF` | Use GLM types instead of native mEn |
| `BUILD_GLFW` | `ON` | Fetch GLFW via FetchContent (OFF = system GLFW) |
| `BUILD_ASSIMP` | `ON` | Fetch assimp via FetchContent (OFF = system assimp) |

## Tests

Only **mEn** has a test suite. Tests use GoogleTest and live in `mEn/tests/`.

```bash
cmake --preset=ci-windows-x64-test
cmake --build --preset=ci-windows-x64-test
ctest --preset ci-windows-x64-test

# Or run directly with a GTest filter
./build/bin/mEn_tests --gtest_filter="Vec3.*"
```

## Architecture

### mEn -- Math Library

Header-only INTERFACE library. Public API: `mEn/src/mEn.hpp`.

- **Vec2/3/4**, **Mat3/4**, **Quat** -- column-major matrices, union-based swizzles (`x/r/s`, `y/g/t`, ...)
- **GLSL-style free functions** -- `dot`, `cross`, `normalize`, `mix`, `clamp`, `perspective`, `lookAt`, `slerp`, and more; all accept scalars and vectors uniformly
- **`detail/vectorize.hpp`** -- `map`, `zip`, `reduce` primitives used to implement generic per-component operations
- Declarations in `.hpp`, implementations in `detail/*.inl` (included at end of each header)
- Optional GLM interop via `MEN_USE_GLM`

### kEn -- Engine Library

Public API: `kEn/src/kEn.hpp`. Entry point: include `kEn/src/kEn/core/entry_point.hpp` and implement `create_application()`.

- **Application** -- singleton; fixed 60 TPS loop; owns `LayerStack` + `Window`
- **Layer stack** -- ordered pipeline: `on_attach` -> `on_update` -> `on_render` -> `on_imgui` -> `on_detach`
- **Event system** -- type-erased `EventDispatcher`; events bubble through layers in reverse order; `KEN_BIND_EVENT_HANDLER()` macro for member callbacks
- **Transform** -- hierarchical with dirty-flag caching; world matrix lazily recomputed and propagated to children
- **Renderer** -- static facade over OpenGL; manages scene data (view/proj matrices, lights); supports point, directional, and spot lights
- **Scene graph** -- `GameObject` static registry (max 6400); parent-child hierarchy; `GameComponent` base for attach/update/render/imgui/event hooks
- **Built-in components** -- `ModelComponent`, `FreeLookComponent`, `FreeMoveComponent`, `LookAtComponent`
- **Cameras** -- `OrthographicCamera`, `PerspectiveCamera`
- **Asset loading** -- OBJ/FBX via assimp; textures via stb_image

## License

See [LICENSE](LICENSE). Third-party license notices are in [THIRD_PARTY_LICENSES/](THIRD_PARTY_LICENSES/).
