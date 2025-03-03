#pragma once
#include <kEn/renderer/buffer.hpp>
#include <kEn/renderer/vertex_array.hpp>

namespace kEn {

class OpenglVertexArray final : public VertexArray {
 public:
  OpenglVertexArray();
  ~OpenglVertexArray() override;

  void bind() const override;
  void unbind() const override;

  void add_vertex_buffer(const std::shared_ptr<VertexBuffer>& vertex_buf) override;
  void set_index_buffer(const std::shared_ptr<IndexBuffer>& index_buf) override;

  const std::vector<std::shared_ptr<VertexBuffer>>& vertex_buffers() const override { return vertex_buffers_; }
  const std::shared_ptr<IndexBuffer>& index_buffer() const override { return index_buffer_; }

 private:
  uint32_t renderer_id_;
  uint32_t vertex_buffer_index_ = 0;

  std::vector<std::shared_ptr<VertexBuffer>> vertex_buffers_;
  std::shared_ptr<IndexBuffer> index_buffer_;
};

}  // namespace kEn
