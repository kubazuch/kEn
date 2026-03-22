#pragma once

#include <cstdint>

#include <kEn/core/core.hpp>
#include <kEn/util/enum_map.hpp>

namespace kEn {

enum class ShaderDataType : std::uint8_t {
  Float,
  Float2,
  Float3,
  Float4,
  Mat3,
  Mat4,
  Int,
  Int2,
  Int3,
  Int4,
  Bool,
  Count
};

namespace shader_data_type {

using enum ShaderDataType;

inline constexpr std::uint32_t kFloatSize = 4;
inline constexpr std::uint32_t kIntSize   = 4;
inline constexpr std::uint32_t kBoolSize  = 1;

inline constexpr util::EnumMap kSizes{{
    std::pair{Float, 1 * kFloatSize},
    std::pair{Float2, 2 * kFloatSize},
    std::pair{Float3, 3 * kFloatSize},
    std::pair{Float4, 4 * kFloatSize},
    std::pair{Mat3, 9 * kFloatSize},
    std::pair{Mat4, 16 * kFloatSize},
    std::pair{Int, 1 * kIntSize},
    std::pair{Int2, 2 * kIntSize},
    std::pair{Int3, 3 * kIntSize},
    std::pair{Int4, 4 * kIntSize},
    std::pair{Bool, kBoolSize},
}};

inline constexpr auto kComponents = util::make_enum_map<std::uint8_t>({
    std::pair{Float, 1},
    std::pair{Float2, 2},
    std::pair{Float3, 3},
    std::pair{Float4, 4},
    std::pair{Mat3, 3},
    std::pair{Mat4, 4},
    std::pair{Int, 1},
    std::pair{Int2, 2},
    std::pair{Int3, 3},
    std::pair{Int4, 4},
    std::pair{Bool, 1},
});

[[nodiscard]] constexpr std::uint32_t get_size(ShaderDataType type) { return kSizes[type]; }

[[nodiscard]] constexpr std::uint8_t get_component_count(ShaderDataType type) { return kComponents[type]; }

}  // namespace shader_data_type

}  // namespace kEn