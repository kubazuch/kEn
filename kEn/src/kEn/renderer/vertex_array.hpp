#pragma once
#include <kEn/renderer/buffer.hpp>

namespace kEn {

class VertexArray {
 public:
  virtual ~VertexArray() = default;

  virtual void bind() const   = 0;
  virtual void unbind() const = 0;

  virtual void add_vertex_buffer(const std::shared_ptr<VertexBuffer>& vertex_buf) = 0;
  virtual void set_index_buffer(const std::shared_ptr<IndexBuffer>& index_buf)    = 0;

  virtual const std::vector<std::shared_ptr<VertexBuffer>>& vertex_buffers() const = 0;
  virtual const std::shared_ptr<IndexBuffer>& index_buffer() const                 = 0;

  static std::unique_ptr<VertexArray> create();
};

}  // namespace kEn
