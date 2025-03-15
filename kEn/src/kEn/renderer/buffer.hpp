#pragma once

#include <string_view>

#define SHADER_FLOAT_SIZE 4
#define SHADER_INT_SIZE 4
#define SHADER_BOOL_SIZE 1

// X-macro of form X(type, size, component count)
#define DATA_TYPES(X)                   \
  X(float_, SHADER_FLOAT_SIZE, 1)       \
  X(float2, 2 * SHADER_FLOAT_SIZE, 2)   \
  X(float3, 3 * SHADER_FLOAT_SIZE, 3)   \
  X(float4, 4 * SHADER_FLOAT_SIZE, 4)   \
  X(mat3, 3 * 3 * SHADER_FLOAT_SIZE, 3) \
  X(mat4, 4 * 4 * SHADER_FLOAT_SIZE, 4) \
  X(int_, SHADER_INT_SIZE, 1)           \
  X(int2, 2 * SHADER_INT_SIZE, 2)       \
  X(int3, 3 * SHADER_INT_SIZE, 3)       \
  X(int4, 4 * SHADER_INT_SIZE, 4)       \
  X(bool_, SHADER_BOOL_SIZE, 1)

#include <kEn/core/assert.hpp>

namespace kEn {

using shader_data_type = uint8_t;

namespace shader_data_types {
#define ENUM_ENTRY(id, size, comps) id,
#define SIZE_ENTRY(id, size, comps) \
  case id:                          \
    return size;
#define COMPS_ENTRY(id, size, comps) \
  case id:                           \
    return comps;

enum : shader_data_type { None = 0, DATA_TYPES(ENUM_ENTRY) };

inline constexpr uint32_t get_size(shader_data_type type) {
  switch (type) { DATA_TYPES(SIZE_ENTRY) }

  KEN_CORE_ASSERT(false, "Unknown shader data type!");
  return 0;
}

inline constexpr uint32_t get_component_count(shader_data_type type) {
  switch (type) { DATA_TYPES(COMPS_ENTRY) }

  KEN_CORE_ASSERT(false, "Unknown shader data type!");
  return 0;
}

#undef ENUM_ENTRY
#undef SIZE_ENTRY
#undef COMPS_ENTRY
}  // namespace shader_data_types

struct BufferElement {
  std::string_view name;
  shader_data_type type{};
  uint32_t size{};
  size_t offset{};
  bool normalized{};

  constexpr BufferElement(shader_data_type type, std::string_view name, bool normalized = false)
      : name(name), type(type), size(shader_data_types::get_size(type)), normalized(normalized) {}
};

struct BufferLayout {
  BufferLayout() = default;

  BufferLayout(std::initializer_list<BufferElement> elements) : elements_(elements) {
    size_t offset = 0;
    for (auto& element : elements_) {
      element.offset = offset;
      offset += element.size;
    }

    stride_ = offset;
  }

  size_t stride() const { return stride_; }
  const std::vector<BufferElement>& elements() const { return elements_; }

  std::vector<BufferElement>::iterator begin() { return elements_.begin(); }
  std::vector<BufferElement>::iterator end() { return elements_.end(); }
  std::vector<BufferElement>::const_iterator begin() const { return elements_.begin(); }
  std::vector<BufferElement>::const_iterator end() const { return elements_.end(); }

 private:
  std::vector<BufferElement> elements_;
  size_t stride_ = 0;
};

struct VertexBuffer {
  virtual ~VertexBuffer() = default;

  virtual void bind() const   = 0;
  virtual void unbind() const = 0;

  virtual const BufferLayout& layout() const          = 0;
  virtual void set_layout(const BufferLayout& layout) = 0;

  static std::shared_ptr<VertexBuffer> create(void* vertices, size_t size);
};

struct MutableVertexBuffer : public VertexBuffer {
  virtual void modify_data(std::function<void(void*)> fn) const = 0;

  static std::shared_ptr<MutableVertexBuffer> create(void* vertices, size_t size);
};

struct IndexBuffer {
  virtual ~IndexBuffer() = default;

  virtual void bind() const   = 0;
  virtual void unbind() const = 0;

  virtual uint32_t get_count() const = 0;

  static std::shared_ptr<IndexBuffer> create(uint32_t* indices, size_t size);
};

}  // namespace kEn