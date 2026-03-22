#pragma once

#include <array>
#include <span>
#include <string_view>

#include <kEn/renderer/buffer.hpp>
#include <kEn/renderer/shader_data_type.hpp>

namespace kEn {

class Shader;

enum class VertexInputRate : std::uint8_t {
  PerVertex,
  PerInstance,
};

enum class IndexType : std::uint8_t {
  UInt16,
  UInt32,
};

struct VertexElement {
  std::string_view semantic;
  ShaderDataType type{};
  std::uint32_t semantic_index{};
  std::size_t offset{};
  bool normalized{};

  constexpr VertexElement() = default;
  constexpr VertexElement(ShaderDataType type, std::string_view semantic, std::size_t offset, bool normalized = false,
                          std::uint32_t semantic_index = 0)
      : semantic(semantic), type(type), semantic_index(semantic_index), offset(offset), normalized(normalized) {}

  [[nodiscard]] constexpr std::size_t size() const { return shader_data_type::get_size(type); }
};

template <std::size_t N>
struct VertexLayout {
  std::array<VertexElement, N> elements{};
  std::size_t stride{};
};

template <class TVertex, std::size_t N>
[[nodiscard]] constexpr auto make_vertex_layout(const VertexElement (&elems)[N]) {
  static_assert(std::is_standard_layout_v<TVertex>, "Vertex types must be standard-layout for offsetof().");
  static_assert(std::is_trivially_copyable_v<TVertex>, "Vertex types must be trivially copyable.");

  return VertexLayout<N>{std::to_array(elems), sizeof(TVertex)};
}

template <class TVertex, std::size_t N>
[[nodiscard]] constexpr auto make_vertex_layout(const VertexElement (&elems)[N], std::size_t stride) {
  static_assert(std::is_standard_layout_v<TVertex>, "Vertex types must be standard-layout for offsetof().");
  static_assert(std::is_trivially_copyable_v<TVertex>, "Vertex types must be trivially copyable.");

  return VertexLayout<N>{std::to_array(elems), stride};
}

struct VertexLayoutView {
  std::span<const VertexElement> elements;
  std::size_t stride{};

  VertexLayoutView() = default;

  constexpr VertexLayoutView(std::span<const VertexElement> elems, std::size_t s) noexcept
      : elements(elems), stride(s) {}

  template <std::size_t N>
  constexpr VertexLayoutView(const VertexLayout<N>& layout) noexcept  // NOLINT(google-explicit-constructor)
      : elements(layout.elements), stride(layout.stride) {}

  [[nodiscard]] constexpr bool empty() const noexcept { return elements.empty(); }
  [[nodiscard]] constexpr auto begin() const noexcept { return elements.begin(); }
  [[nodiscard]] constexpr auto end() const noexcept { return elements.end(); }
};

struct VertexStreamBinding {
  std::shared_ptr<Buffer> buffer;
  VertexLayoutView layout;
  std::size_t buffer_offset{};
  VertexInputRate input_rate{VertexInputRate::PerVertex};
  std::uint32_t instance_step_rate{1};
};

class VertexInput {
 public:
  virtual ~VertexInput() = default;

  virtual void bind() const   = 0;
  virtual void unbind() const = 0;

  virtual void add_vertex_stream(const VertexStreamBinding& stream) = 0;
  [[nodiscard]] virtual size_t element_count() const                = 0;

  void set_index_buffer(const std::shared_ptr<Buffer>& index_buf, IndexType index_type = IndexType::UInt32,
                        std::size_t index_offset = 0) {
    set_index_buffer_impl(index_buf, index_type, index_offset);
  }

  [[nodiscard]] virtual std::span<const VertexStreamBinding> vertex_streams() const = 0;
  [[nodiscard]] virtual const std::shared_ptr<Buffer>& index_buffer() const         = 0;
  [[nodiscard]] virtual IndexType index_type() const                                = 0;
  [[nodiscard]] virtual size_t index_buffer_offset() const                          = 0;

 protected:
  virtual void set_index_buffer_impl(const std::shared_ptr<Buffer>& index_buf, IndexType index_type,
                                     std::size_t index_offset) = 0;
};

}  // namespace kEn
