#pragma once

#include <kEn/renderer/buffer.hpp>
#include <memory>

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

  friend class OpenglUniformBuffer;
  friend class OpenglShaderStorageBuffer;
};

class OpenglMutableBuffer final : public MutableBuffer, public OpenglBuffer {
 public:
  OpenglMutableBuffer(const void* data, size_t size) : OpenglBuffer(data, size) {}

  void modify_data(std::function<void(void*)> fn) const override;
  void set_data(const void* data, size_t size) override;

 private:
  void set_data_int(const void* data, size_t size) const override;
};

class OpenglUniformBuffer final : public UniformBuffer {
 public:
  OpenglUniformBuffer(std::shared_ptr<OpenglBuffer> buffer, size_t binding_point);

  void bind() const override { buffer_->bind(BufferType::Uniform); }
  void unbind() const override { buffer_->unbind(BufferType::Uniform); }

  std::shared_ptr<Buffer> underlying_buffer() const override { return std::static_pointer_cast<Buffer>(buffer_); };
  size_t binding_point() const override { return binding_point_; }

 private:
  std::shared_ptr<OpenglBuffer> buffer_;
  size_t binding_point_;
};

class OpenglShaderStorageBuffer final : public ShaderStorageBuffer {
 public:
  OpenglShaderStorageBuffer(std::shared_ptr<OpenglBuffer> buffer, size_t binding_point);

  void bind() const override { buffer_->bind(BufferType::ShaderStorage); }
  void unbind() const override { buffer_->unbind(BufferType::ShaderStorage); }

  std::shared_ptr<Buffer> underlying_buffer() const override { return std::static_pointer_cast<Buffer>(buffer_); };
  size_t binding_point() const override { return binding_point_; }

 private:
  std::shared_ptr<OpenglBuffer> buffer_;
  size_t binding_point_;
};

}  // namespace kEn