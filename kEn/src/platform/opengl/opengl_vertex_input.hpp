#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <span>
#include <vector>

#include <kEn/renderer/vertex_input.hpp>

namespace kEn {

class OpenglVertexInput final : public VertexInput {
 public:
  OpenglVertexInput();
  ~OpenglVertexInput() override;

  [[nodiscard]] std::uintptr_t native_handle() const noexcept override {
    return static_cast<std::uintptr_t>(renderer_id_);
  }

  void add_vertex_stream(const VertexStreamBinding& stream) override;
  [[nodiscard]] size_t element_count() const override;

  [[nodiscard]] std::span<const VertexStreamBinding> vertex_streams() const override;
  [[nodiscard]] const std::shared_ptr<Buffer>& index_buffer() const override;
  [[nodiscard]] IndexType index_type() const override;
  [[nodiscard]] size_t index_buffer_offset() const override;

 protected:
  void set_index_buffer_impl(std::shared_ptr<Buffer> index_buf, IndexType index_type,
                             std::size_t index_offset) override;

 private:
  uint32_t renderer_id_{0};
  uint32_t attrib_index_{0};

  std::vector<VertexStreamBinding> streams_;
  std::shared_ptr<Buffer> index_buffer_;
  IndexType index_type_{IndexType::UInt32};
  std::size_t index_buffer_offset_{0};
};

}  // namespace kEn
