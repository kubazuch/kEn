#include "opengl_buffer.hpp"

#include <glad/gl.h>

#include <cstddef>
#include <memory>
#include <span>
#include <utility>

#include <kEn/core/assert.hpp>
#include <kEn/renderer/buffer.hpp>
#include <kEn/renderer/shader.hpp>

namespace kEn {

OpenglBuffer::OpenglBuffer(const BufferDesc& desc, const void* data) : desc_(desc) {
  glCreateBuffers(1, &renderer_id_);
  allocate(data);
}

OpenglBuffer::OpenglBuffer(const void* data, std::size_t size, BufferUsage usage, BufferBinds bind_flags)
    : OpenglBuffer({.size = size, .usage = usage, .bind_flags = bind_flags}, data) {}

OpenglBuffer::~OpenglBuffer() {
  if (renderer_id_ != 0) {
    glDeleteBuffers(1, &renderer_id_);
  }
}

OpenglBuffer::OpenglBuffer(OpenglBuffer&& other) noexcept
    : desc_(other.desc_), renderer_id_(std::exchange(other.renderer_id_, 0)) {
  other.desc_ = {};
}

OpenglBuffer& OpenglBuffer::operator=(OpenglBuffer&& other) noexcept {
  if (this == &other) {
    return *this;
  }

  if (renderer_id_ != 0) {
    glDeleteBuffers(1, &renderer_id_);
  }

  desc_        = other.desc_;
  renderer_id_ = std::exchange(other.renderer_id_, 0);
  other.desc_  = {};
  return *this;
}

void OpenglBuffer::bind(BufferTarget target) const { glBindBuffer(buffer_target::opengl_target(target), renderer_id_); }

void OpenglBuffer::unbind(BufferTarget target) const {  // NOLINT(readability-convert-member-functions-to-static)
  glBindBuffer(buffer_target::opengl_target(target), 0);
}

void OpenglBuffer::allocate(const void* data) const {
  glNamedBufferData(renderer_id_, static_cast<GLsizeiptr>(desc_.size), data, buffer_usage::opengl_usage(desc_.usage));
}

// ---------------- //

OpenglMutableBuffer::OpenglMutableBuffer(std::shared_ptr<OpenglBuffer> buffer) : buffer_(std::move(buffer)) {
  KEN_CORE_ASSERT(buffer_ != nullptr, "OpenglMutableBuffer requires a valid buffer");
  KEN_CORE_ASSERT(buffer_->desc_.usage != BufferUsage::Immutable, "OpenglMutableBuffer requires mutable buffer");
}

OpenglMutableBuffer::OpenglMutableBuffer(const BufferDesc& desc, const void* data)
    : buffer_(std::make_shared<OpenglBuffer>(desc, data)) {}

OpenglMutableBuffer::OpenglMutableBuffer(const void* data, std::size_t size, BufferUsage usage, BufferBinds bind_flags)
    : buffer_(std::make_shared<OpenglBuffer>(data, size, usage, bind_flags)) {}

bool OpenglMutableBuffer::can_map(MapMode mode) const {
  switch (buffer_->desc_.usage) {
    case BufferUsage::Immutable:
    case BufferUsage::Default:
      return false;
    case BufferUsage::Dynamic:
      return mode != MapMode::Read && mode != MapMode::ReadWrite;
    case BufferUsage::Staging:
      return true;
  }

  KEN_UNREACHABLE();
}

MappedBuffer OpenglMutableBuffer::map(MapMode mode) const {
  KEN_CORE_ASSERT(can_map(mode), "Requested map mode is incompatible with buffer usage");

  void* ptr = glMapNamedBufferRange(buffer_->renderer_id_, 0, static_cast<GLsizeiptr>(buffer_->size()),
                                    map_mode::opengl_mode(mode));
  KEN_CORE_ASSERT(ptr != nullptr, "glMapNamedBufferRange failed");

  const GLuint id = buffer_->renderer_id_;
  return MappedBuffer(std::span<std::byte>(static_cast<std::byte*>(ptr), buffer_->size()),
                      [id]() { glUnmapNamedBuffer(id); });
}

void OpenglMutableBuffer::set_data(const void* data, std::size_t size) {
  if (size != buffer_->size()) {
    resize(size, data);
    return;
  }

  update_data(0, data, size);
}

void OpenglMutableBuffer::update_data(std::size_t offset, const void* data, std::size_t size) {
  KEN_CORE_ASSERT(offset + size <= buffer_->size(), "Buffer update range is out of bounds");

  if (size == 0) {
    return;
  }

  glNamedBufferSubData(buffer_->renderer_id_, static_cast<GLintptr>(offset), static_cast<GLsizeiptr>(size), data);
}

void OpenglMutableBuffer::resize(std::size_t size, const void* data) {
  buffer_->set_size(size);
  buffer_->allocate(data);
}

// ---------------- //

OpenglUniformBuffer::OpenglUniformBuffer(std::shared_ptr<OpenglBuffer> buffer, std::size_t slot, ShaderStage stage)
    : buffer_(std::move(buffer)), slot_(slot), stage_(stage) {
  KEN_CORE_ASSERT(buffer_ != nullptr, "OpenglUniformBuffer requires a valid buffer");
  KEN_CORE_ASSERT(buffer_->desc().bind_flags.test(BufferBind::Uniform),
                  "Underlying buffer is not marked for uniform binding");
}

void OpenglUniformBuffer::bind() const {
  glBindBufferBase(GL_UNIFORM_BUFFER, static_cast<GLuint>(slot_), buffer_->renderer_id());
}

void OpenglUniformBuffer::unbind() const { glBindBufferBase(GL_UNIFORM_BUFFER, static_cast<GLuint>(slot_), 0); }

// ---------------- //

OpenglShaderStorageBuffer::OpenglShaderStorageBuffer(std::shared_ptr<OpenglBuffer> buffer, std::size_t slot,
                                                     ShaderStage stage)
    : buffer_(std::move(buffer)), slot_(slot), stage_(stage) {
  KEN_CORE_ASSERT(buffer_ != nullptr, "OpenglShaderStorageBuffer requires a valid buffer");
  KEN_CORE_ASSERT(buffer_->desc().bind_flags.test(BufferBind::Storage),
                  "Underlying buffer is not marked for storage binding");
}

void OpenglShaderStorageBuffer::bind() const {
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, static_cast<GLuint>(slot_), buffer_->renderer_id());
}

void OpenglShaderStorageBuffer::unbind() const {
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, static_cast<GLuint>(slot_), 0);
}

}  // namespace kEn
