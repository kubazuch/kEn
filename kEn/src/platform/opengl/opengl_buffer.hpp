#pragma once

#include <kEn/renderer/buffer.hpp>

namespace kEn {

class OpenglBuffer : virtual public Buffer {
 public:
  OpenglBuffer(const void* data, size_t size);
  ~OpenglBuffer() override;

  void bind(BufferType type) const override;
  void unbind(BufferType type) const override;

  const BufferLayout& layout() const override { return layout_; }
  void set_layout(const BufferLayout& layout) override { layout_ = layout; }
  size_t size() const override { return size_; }

 protected:
  virtual void set_data_int(const void* data, size_t size) const;

  size_t size_;
  uint32_t renderer_id_;
  BufferLayout layout_;
};

class OpenglMutableBuffer final : public MutableBuffer, public OpenglBuffer {
 public:
  OpenglMutableBuffer(const void* data, size_t size) : OpenglBuffer(data, size) {}

  void modify_data(std::function<void(void*)> fn) const override;
  void set_data(const void* data, size_t size) override;

 private:
  void set_data_int(const void* data, size_t size) const override;
};

}  // namespace kEn