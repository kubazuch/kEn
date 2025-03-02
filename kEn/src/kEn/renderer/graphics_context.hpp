#pragma once

namespace kEn {

class graphics_context {
 public:
  virtual ~graphics_context() = default;

  virtual void init()         = 0;
  virtual void swap_buffers() = 0;

  static std::unique_ptr<graphics_context> create(void* native_window);
};

}  // namespace kEn
