#pragma once

#include <cstdint>

#include <kEn/core/core.hpp>

/** @file
 *  @ingroup ken
 *  @brief Pipeline state descriptors and abstract GPU state objects.
 *
 *  State objects follow a D3D11-style pattern:
 *  1. Fill a `*Desc` struct describing the desired pipeline state.
 *  2. Call `Device::create_*_state(desc)` once to compile an immutable GPU object.
 *  3. Pass that object to `RenderContext::set_*_state()` at draw time.
 *
 *  For the OpenGL backend the objects simply cache the descriptor and re-apply
 *  it imperatively on bind.  A future D3D11 backend would wrap an
 *  `ID3D11*State*` COM pointer inside the concrete subclass.
 */

namespace kEn {

// ---------------------------------------------------------------------------
// Enumerations
// ---------------------------------------------------------------------------

/** @brief Comparison function used for depth, stencil, and sampler comparisons. */
enum class CompareOp : std::uint8_t {
  Never,        /**< @brief Comparison never passes. */
  Less,         /**< @brief Passes if incoming < reference. */
  Equal,        /**< @brief Passes if incoming == reference. */
  LessEqual,    /**< @brief Passes if incoming <= reference. */
  Greater,      /**< @brief Passes if incoming > reference. */
  NotEqual,     /**< @brief Passes if incoming != reference. */
  GreaterEqual, /**< @brief Passes if incoming >= reference. */
  Always,       /**< @brief Comparison always passes. */
};

/** @brief Operation applied to the stencil buffer. */
enum class StencilOp : std::uint8_t {
  Keep,     /**< @brief Keep the current stencil value. */
  Zero,     /**< @brief Set the stencil value to zero. */
  Replace,  /**< @brief Replace with the reference value. */
  IncrSat,  /**< @brief Increment and clamp to the maximum value. */
  DecrSat,  /**< @brief Decrement and clamp to zero. */
  Invert,   /**< @brief Bitwise-invert the stencil value. */
  IncrWrap, /**< @brief Increment and wrap to zero on overflow. */
  DecrWrap, /**< @brief Decrement and wrap to the maximum on underflow. */
};

/** @brief Source or destination factor in a blend equation. */
enum class BlendFactor : std::uint8_t {
  Zero,                  /**< @brief Factor is 0. */
  One,                   /**< @brief Factor is 1. */
  SrcColor,              /**< @brief Factor is the source colour. */
  OneMinusSrcColor,      /**< @brief Factor is 1 minus the source colour. */
  DstColor,              /**< @brief Factor is the destination colour. */
  OneMinusDstColor,      /**< @brief Factor is 1 minus the destination colour. */
  SrcAlpha,              /**< @brief Factor is the source alpha. */
  OneMinusSrcAlpha,      /**< @brief Factor is 1 minus the source alpha. */
  DstAlpha,              /**< @brief Factor is the destination alpha. */
  OneMinusDstAlpha,      /**< @brief Factor is 1 minus the destination alpha. */
  ConstantColor,         /**< @brief Factor is the blend constant colour. */
  OneMinusConstantColor, /**< @brief Factor is 1 minus the blend constant colour. */
  ConstantAlpha,         /**< @brief Factor is the blend constant alpha. */
  OneMinusConstantAlpha, /**< @brief Factor is 1 minus the blend constant alpha. */
  SrcAlphaSaturate,      /**< @brief Factor is min(src_alpha, 1 - dst_alpha). */
};

/** @brief Arithmetic operation applied to the two blended colour terms. */
enum class BlendOp : std::uint8_t {
  Add,             /**< @brief result = src * src_factor + dst * dst_factor. */
  Subtract,        /**< @brief result = src * src_factor - dst * dst_factor. */
  ReverseSubtract, /**< @brief result = dst * dst_factor - src * src_factor. */
  Min,             /**< @brief result = min(src, dst). */
  Max,             /**< @brief result = max(src, dst). */
};

/** @brief Polygon rasterization fill mode. */
enum class FillMode : std::uint8_t {
  Solid,     /**< @brief Filled polygon (default). */
  Wireframe, /**< @brief Wireframe / line edges only. */
};

/** @brief Winding order of front-facing triangles. */
enum class FrontFace : std::uint8_t {
  Ccw, /**< @brief Counter-clockwise winding is front-facing (default, OpenGL convention). */
  Cw,  /**< @brief Clockwise winding is front-facing. */
};

/** @brief Which triangle faces are culled during rasterization. */
enum class CullMode : std::uint8_t {
  None,  /**< @brief No face culling. */
  Front, /**< @brief Cull front-facing triangles. */
  Back,  /**< @brief Cull back-facing triangles (default). */
};

// ---------------------------------------------------------------------------
// Stencil face descriptor
// ---------------------------------------------------------------------------

/**
 * @brief Per-face stencil operation and comparison descriptor.
 *
 * Used as a sub-struct of @ref DepthStateDesc for the front and back faces.
 */
struct StencilFaceDesc {
  StencilOp fail_op       = StencilOp::Keep;   /**< @brief Op when stencil test fails. */
  StencilOp depth_fail_op = StencilOp::Keep;   /**< @brief Op when depth test fails. */
  StencilOp pass_op       = StencilOp::Keep;   /**< @brief Op when both tests pass. */
  CompareOp compare_op    = CompareOp::Always; /**< @brief Stencil comparison function. */
  std::uint8_t reference  = 0;                 /**< @brief Reference value for comparison and replace. */
  std::uint8_t read_mask  = 0xFF;              /**< @brief Mask ANDed with stencil before comparison. */
  std::uint8_t write_mask = 0xFF;              /**< @brief Mask controlling which stencil bits are written. */
};

// ---------------------------------------------------------------------------
// State descriptors
// ---------------------------------------------------------------------------

/**
 * @brief Descriptor for the depth/stencil pipeline stage.
 *
 * Default values replicate the initial OpenGL state set by @c OpenglRenderContext::init().
 */
struct DepthStateDesc {
  bool depth_test_enabled    = true;            /**< @brief Enable depth testing. */
  bool depth_write_enabled   = true;            /**< @brief Enable depth buffer writes. */
  CompareOp depth_compare_op = CompareOp::Less; /**< @brief Depth comparison function. */
  bool stencil_test_enabled  = false;           /**< @brief Enable stencil testing. */
  StencilFaceDesc front_face;                   /**< @brief Stencil ops for front-facing fragments. */
  StencilFaceDesc back_face;                    /**< @brief Stencil ops for back-facing fragments. */
};

/**
 * @brief Descriptor for the output-merger blend stage.
 *
 * Default values replicate the initial OpenGL state set by @c OpenglRenderContext::init().
 * Color write masking is placed here as it belongs to the blend / output-merger stage.
 */
struct BlendStateDesc {
  bool blend_enabled    = true;                          /**< @brief Enable alpha blending. */
  BlendFactor src_color = BlendFactor::SrcAlpha;         /**< @brief Source RGB blend factor. */
  BlendFactor dst_color = BlendFactor::OneMinusSrcAlpha; /**< @brief Destination RGB blend factor. */
  BlendOp color_op      = BlendOp::Add;                  /**< @brief RGB blend operation. */
  BlendFactor src_alpha = BlendFactor::One;              /**< @brief Source alpha blend factor. */
  BlendFactor dst_alpha = BlendFactor::Zero;             /**< @brief Destination alpha blend factor. */
  BlendOp alpha_op      = BlendOp::Add;                  /**< @brief Alpha blend operation. */
  bool write_r          = true;                          /**< @brief Enable red channel writes. */
  bool write_g          = true;                          /**< @brief Enable green channel writes. */
  bool write_b          = true;                          /**< @brief Enable blue channel writes. */
  bool write_a          = true;                          /**< @brief Enable alpha channel writes. */
};

/**
 * @brief Descriptor for the rasterizer stage.
 *
 * Default values replicate the initial OpenGL state set by @c OpenglRenderContext::init().
 */
struct RasterStateDesc {
  FillMode fill_mode        = FillMode::Solid; /**< @brief Polygon fill mode. */
  CullMode cull_mode        = CullMode::Back;  /**< @brief Face culling mode. */
  FrontFace front_face      = FrontFace::Ccw;  /**< @brief Front-face winding order. */
  bool scissor_test_enabled = false;           /**< @brief Enable scissor testing. */
  float depth_bias_constant = 0.0F;            /**< @brief Constant depth bias added to each fragment. */
  float depth_bias_slope    = 0.0F;            /**< @brief Slope-scaled depth bias. */
};

// ---------------------------------------------------------------------------
// Abstract GPU state objects
// ---------------------------------------------------------------------------

/**
 * @brief Immutable GPU object representing compiled depth/stencil state.
 *
 * Obtain an instance via @ref Device::create_depth_state().
 * Bind it with @ref RenderContext::set_depth_state().
 */
class DepthState {
 public:
  virtual ~DepthState() = default;
};

/**
 * @brief Immutable GPU object representing compiled blend state.
 *
 * Obtain an instance via @ref Device::create_blend_state().
 * Bind it with @ref RenderContext::set_blend_state().
 */
class BlendState {
 public:
  virtual ~BlendState() = default;
};

/**
 * @brief Immutable GPU object representing compiled rasterizer state.
 *
 * Obtain an instance via @ref Device::create_raster_state().
 * Bind it with @ref RenderContext::set_raster_state().
 */
class RasterState {
 public:
  virtual ~RasterState() = default;
};

}  // namespace kEn
