#pragma once

#include <glad/gl.h>

#include <kEn/renderer/render_state.hpp>
#include <kEn/util/enum_map.hpp>

namespace kEn {

// ---------------------------------------------------------------------------
// GL enum maps and conversion helpers
// ---------------------------------------------------------------------------

namespace render_state {

namespace detail {

inline constexpr auto kOpenglCompareOps = util::make_enum_map<GLenum>({
    std::pair{CompareOp::Never, GL_NEVER},
    std::pair{CompareOp::Less, GL_LESS},
    std::pair{CompareOp::Equal, GL_EQUAL},
    std::pair{CompareOp::LessEqual, GL_LEQUAL},
    std::pair{CompareOp::Greater, GL_GREATER},
    std::pair{CompareOp::NotEqual, GL_NOTEQUAL},
    std::pair{CompareOp::GreaterEqual, GL_GEQUAL},
    std::pair{CompareOp::Always, GL_ALWAYS},
});

inline constexpr auto kOpenglStencilOps = util::make_enum_map<GLenum>({
    std::pair{StencilOp::Keep, GL_KEEP},
    std::pair{StencilOp::Zero, GL_ZERO},
    std::pair{StencilOp::Replace, GL_REPLACE},
    std::pair{StencilOp::IncrSat, GL_INCR},
    std::pair{StencilOp::DecrSat, GL_DECR},
    std::pair{StencilOp::Invert, GL_INVERT},
    std::pair{StencilOp::IncrWrap, GL_INCR_WRAP},
    std::pair{StencilOp::DecrWrap, GL_DECR_WRAP},
});

inline constexpr auto kOpenglBlendFactors = util::make_enum_map<GLenum>({
    std::pair{BlendFactor::Zero, GL_ZERO},
    std::pair{BlendFactor::One, GL_ONE},
    std::pair{BlendFactor::SrcColor, GL_SRC_COLOR},
    std::pair{BlendFactor::OneMinusSrcColor, GL_ONE_MINUS_SRC_COLOR},
    std::pair{BlendFactor::DstColor, GL_DST_COLOR},
    std::pair{BlendFactor::OneMinusDstColor, GL_ONE_MINUS_DST_COLOR},
    std::pair{BlendFactor::SrcAlpha, GL_SRC_ALPHA},
    std::pair{BlendFactor::OneMinusSrcAlpha, GL_ONE_MINUS_SRC_ALPHA},
    std::pair{BlendFactor::DstAlpha, GL_DST_ALPHA},
    std::pair{BlendFactor::OneMinusDstAlpha, GL_ONE_MINUS_DST_ALPHA},
    std::pair{BlendFactor::ConstantColor, GL_CONSTANT_COLOR},
    std::pair{BlendFactor::OneMinusConstantColor, GL_ONE_MINUS_CONSTANT_COLOR},
    std::pair{BlendFactor::ConstantAlpha, GL_CONSTANT_ALPHA},
    std::pair{BlendFactor::OneMinusConstantAlpha, GL_ONE_MINUS_CONSTANT_ALPHA},
    std::pair{BlendFactor::SrcAlphaSaturate, GL_SRC_ALPHA_SATURATE},
});

inline constexpr auto kOpenglBlendOps = util::make_enum_map<GLenum>({
    std::pair{BlendOp::Add, GL_FUNC_ADD},
    std::pair{BlendOp::Subtract, GL_FUNC_SUBTRACT},
    std::pair{BlendOp::ReverseSubtract, GL_FUNC_REVERSE_SUBTRACT},
    std::pair{BlendOp::Min, GL_MIN},
    std::pair{BlendOp::Max, GL_MAX},
});

}  // namespace detail

[[nodiscard]] constexpr GLenum opengl_compare_op(CompareOp op) { return detail::kOpenglCompareOps[op]; }
[[nodiscard]] constexpr GLenum opengl_stencil_op(StencilOp op) { return detail::kOpenglStencilOps[op]; }
[[nodiscard]] constexpr GLenum opengl_blend_factor(BlendFactor f) { return detail::kOpenglBlendFactors[f]; }
[[nodiscard]] constexpr GLenum opengl_blend_op(BlendOp op) { return detail::kOpenglBlendOps[op]; }

}  // namespace render_state

// ---------------------------------------------------------------------------
// Concrete state objects
// ---------------------------------------------------------------------------

/** @brief OpenGL concrete depth/stencil state object.  Stores the descriptor and applies it on bind. */
class OpenglDepthState final : public DepthState {
 public:
  explicit OpenglDepthState(const DepthStateDesc& desc) : desc_(desc) {}
  [[nodiscard]] const DepthStateDesc& desc() const { return desc_; }

 private:
  DepthStateDesc desc_;
};

/** @brief OpenGL concrete blend state object.  Stores the descriptor and applies it on bind. */
class OpenglBlendState final : public BlendState {
 public:
  explicit OpenglBlendState(const BlendStateDesc& desc) : desc_(desc) {}
  [[nodiscard]] const BlendStateDesc& desc() const { return desc_; }

 private:
  BlendStateDesc desc_;
};

/** @brief OpenGL concrete rasterizer state object.  Stores the descriptor and applies it on bind. */
class OpenglRasterState final : public RasterState {
 public:
  explicit OpenglRasterState(const RasterStateDesc& desc) : desc_(desc) {}
  [[nodiscard]] const RasterStateDesc& desc() const { return desc_; }

 private:
  RasterStateDesc desc_;
};

}  // namespace kEn
