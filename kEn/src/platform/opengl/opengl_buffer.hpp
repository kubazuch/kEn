#pragma once

#include <kEn/renderer/buffer.hpp>

namespace kEn {

class OpenglVertexBuffer : public VertexBuffer {
 public:
  OpenglVertexBuffer(void* vertices, uint32_t size);
  ~OpenglVertexBuffer() override;

  void bind() const override;
  void unbind() const override;

  const BufferLayout& layout() const override { return layout_; }
  void set_layout(const BufferLayout& layout) override { layout_ = layout; }

 private:
  uint32_t renderer_id_;
  BufferLayout layout_;
};

class OpenglMutableVertexBuffer final : public MutableVertexBuffer {
 public:
  OpenglMutableVertexBuffer(void* vertices, uint32_t size);
  ~OpenglMutableVertexBuffer() override;

  void bind() const override;
  void unbind() const override;
  void modify_data(std::function<void(void*)> fn) const override;

  const BufferLayout& layout() const override { return layout_; }
  void set_layout(const BufferLayout& layout) override { layout_ = layout; }

 private:
  uint32_t renderer_id_;
  BufferLayout layout_;
};

class OpenglIndexBuffer final : public IndexBuffer {
 public:
  OpenglIndexBuffer(uint32_t* indices, uint32_t count);
  ~OpenglIndexBuffer() override;

  void bind() const override;
  void unbind() const override;

  uint32_t get_count() const override { return count_; }

 private:
  uint32_t renderer_id_;
  uint32_t count_;
};

class OpenglMutableIndexBuffer final : public MutableIndexBuffer {
 public:
  OpenglMutableIndexBuffer(uint32_t* indices, uint32_t count);
  ~OpenglMutableIndexBuffer() override;

  void bind() const override;
  void unbind() const override;
  void modify_data(std::function<void(void*)> fn) const override;

  uint32_t get_count() const override { return count_; }

 private:
  uint32_t renderer_id_;
  uint32_t count_;
};

}  // namespace kEn