#pragma once

#include <array>
#include <span>
#include <string_view>

#include <kEn/renderer/buffer.hpp>
#include <kEn/renderer/shader_data_type.hpp>

/** @file
 *  @ingroup ken
 */

namespace kEn {

class Shader;

/** @brief Controls whether a vertex stream advances per vertex or per instance. */
enum class VertexInputRate : std::uint8_t {
  PerVertex,   /**< @brief Advance the buffer offset by one element per vertex. */
  PerInstance, /**< @brief Advance the buffer offset by one element per instance. */
};

/** @brief Index element type used in an index buffer. */
enum class IndexType : std::uint8_t {
  UInt16, /**< @brief 16-bit unsigned integer indices. */
  UInt32, /**< @brief 32-bit unsigned integer indices. */
};

/** @brief Describes a single vertex attribute within a vertex buffer stream. */
struct VertexElement {
  std::string_view semantic;      /**< @brief Semantic name (e.g. "POSITION", "TEXCOORD"). */
  ShaderDataType type{};          /**< @brief Data type and component count of the attribute. */
  std::uint32_t semantic_index{}; /**< @brief Disambiguates multiple attributes with the same semantic. */
  std::size_t offset{};           /**< @brief Byte offset of the attribute within one vertex. */
  bool normalized{};              /**< @brief Whether integer data should be normalized to [0, 1] / [-1, 1]. */

  constexpr VertexElement() = default;
  constexpr VertexElement(ShaderDataType type, std::string_view semantic, std::size_t offset, bool normalized = false,
                          std::uint32_t semantic_index = 0)
      : semantic(semantic), type(type), semantic_index(semantic_index), offset(offset), normalized(normalized) {}

  /** @brief Returns the byte size of this attribute as reported by shader_data_type::size(). */
  [[nodiscard]] constexpr std::size_t size() const { return shader_data_type::size(type); }
};

/**
 * @brief Compile-time array of vertex elements together with a vertex stride.
 *
 * @tparam N Number of vertex attributes.
 */
template <std::size_t N>
struct VertexLayout {
  std::array<VertexElement, N> elements{}; /**< @brief Ordered list of vertex attribute descriptors. */
  std::size_t stride{};                    /**< @brief Byte distance between successive vertices. */
};

/**
 * @brief Build a VertexLayout for vertex type @p TVertex from a C-array of elements.
 *
 * The stride is derived from @c sizeof(TVertex).
 *
 * @tparam TVertex Vertex struct type; must be standard-layout and trivially copyable.
 * @tparam N       Number of elements (deduced).
 * @param elems    C-array of VertexElement descriptors.
 */
template <class TVertex, std::size_t N>
[[nodiscard]] constexpr auto make_vertex_layout(const VertexElement (&elems)[N]) {
  static_assert(std::is_standard_layout_v<TVertex>, "Vertex types must be standard-layout for offsetof().");
  static_assert(std::is_trivially_copyable_v<TVertex>, "Vertex types must be trivially copyable.");

  return VertexLayout<N>{std::to_array(elems), sizeof(TVertex)};
}

/**
 * @brief Build a VertexLayout with an explicit stride.
 *
 * @tparam TVertex Vertex struct type; must be standard-layout and trivially copyable.
 * @tparam N       Number of elements (deduced).
 * @param elems    C-array of VertexElement descriptors.
 * @param stride   Explicit byte stride between successive vertices.
 */
template <class TVertex, std::size_t N>
[[nodiscard]] constexpr auto make_vertex_layout(const VertexElement (&elems)[N], std::size_t stride) {
  static_assert(std::is_standard_layout_v<TVertex>, "Vertex types must be standard-layout for offsetof().");
  static_assert(std::is_trivially_copyable_v<TVertex>, "Vertex types must be trivially copyable.");

  return VertexLayout<N>{std::to_array(elems), stride};
}

/**
 * @brief Non-owning view of a vertex layout (span of elements + stride).
 *
 * Allows uniform handling of VertexLayout<N> instances of different sizes.
 * Implicitly constructible from any VertexLayout<N>.
 */
struct VertexLayoutView {
  std::span<const VertexElement> elements; /**< @brief View over the attribute descriptors. */
  std::size_t stride{};                    /**< @brief Byte stride between successive vertices. */

  VertexLayoutView() = default;

  /** @brief Construct from a pre-built span and explicit stride. */
  constexpr VertexLayoutView(std::span<const VertexElement> elems, std::size_t s) noexcept
      : elements(elems), stride(s) {}

  /** @brief Implicit conversion from a typed VertexLayout<N>. */
  template <std::size_t N>
  constexpr VertexLayoutView(const VertexLayout<N>& layout) noexcept  // NOLINT(google-explicit-constructor)
      : elements(layout.elements), stride(layout.stride) {}

  /** @brief Returns true if the element span is empty. */
  [[nodiscard]] constexpr bool empty() const noexcept { return elements.empty(); }
  /** @brief Returns an iterator to the first vertex element. */
  [[nodiscard]] constexpr auto begin() const noexcept { return elements.begin(); }
  /** @brief Returns a past-the-end iterator over the vertex elements. */
  [[nodiscard]] constexpr auto end() const noexcept { return elements.end(); }
};

/** @brief Binds a Buffer to a specific vertex stream slot, together with its layout and instancing parameters. */
struct VertexStreamBinding {
  std::shared_ptr<Buffer> buffer; /**< @brief Source vertex buffer. */
  VertexLayoutView layout;        /**< @brief Attribute layout for this stream. */
  std::size_t buffer_offset{};    /**< @brief Byte offset into the buffer where vertex data starts. */
  VertexInputRate input_rate{VertexInputRate::PerVertex}; /**< @brief Per-vertex or per-instance advancement. */
  std::uint32_t instance_step_rate{1}; /**< @brief How many instances share one element (PerInstance only). */
};

/**
 * @brief Abstract vertex-input state object (VAO equivalent).
 *
 * Owns a set of vertex stream bindings and an optional index buffer.
 * Platform-specific subclasses translate these into the native API state.
 */
class VertexInput {
 public:
  virtual ~VertexInput() = default;

  /** @brief Return the platform-native vertex-input handle. */
  [[nodiscard]] virtual std::uintptr_t native_handle() const noexcept = 0;

  /** @brief Append a vertex stream binding to this input layout. */
  virtual void add_vertex_stream(const VertexStreamBinding& stream) = 0;
  /** @brief Return the total number of vertex elements across all streams. */
  [[nodiscard]] virtual std::size_t element_count() const = 0;

  /**
   * @brief Attach an index buffer.
   * @param index_buf    Shared buffer containing index data.
   * @param index_type   Element type (UInt16 or UInt32).
   * @param index_offset Byte offset into @p index_buf where indices start.
   */
  void set_index_buffer(std::shared_ptr<Buffer> index_buf, IndexType index_type = IndexType::UInt32,
                        std::size_t index_offset = 0) {
    set_index_buffer_impl(std::move(index_buf), index_type, index_offset);
  }

  /** @brief Return a span over all vertex stream bindings. */
  [[nodiscard]] virtual std::span<const VertexStreamBinding> vertex_streams() const = 0;
  /** @brief Return the currently attached index buffer (may be null). */
  [[nodiscard]] virtual const std::shared_ptr<Buffer>& index_buffer() const = 0;
  /** @brief Return the index element type. */
  [[nodiscard]] virtual IndexType index_type() const = 0;
  /** @brief Return the byte offset into the index buffer where indices start. */
  [[nodiscard]] virtual std::size_t index_buffer_offset() const = 0;

 protected:
  /** @brief Platform-specific implementation of set_index_buffer(). */
  virtual void set_index_buffer_impl(std::shared_ptr<Buffer> index_buf, IndexType index_type,
                                     std::size_t index_offset) = 0;
};

}  // namespace kEn
