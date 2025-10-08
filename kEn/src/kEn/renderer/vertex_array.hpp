#pragma once
#include <kEn/renderer/buffer.hpp>

namespace kEn {

class VertexArray {
 public:
  virtual ~VertexArray() = default;

  virtual void bind() const   = 0;
  virtual void unbind() const = 0;

  virtual void add_vertex_buffer(const std::shared_ptr<Buffer>& vertex_buf)                 = 0;
  virtual void add_vertex_buffer(const std::shared_ptr<Buffer>& vertex_buf, size_t divisor) = 0;
  virtual void set_index_buffer(const std::shared_ptr<Buffer>& index_buf)                   = 0;
  virtual size_t element_count() const                                                      = 0;

  virtual const std::vector<std::shared_ptr<Buffer>>& vertex_buffers() const = 0;
  virtual const std::shared_ptr<Buffer>& index_buffer() const                = 0;

  static std::unique_ptr<VertexArray> create();
};

}  // namespace kEn
