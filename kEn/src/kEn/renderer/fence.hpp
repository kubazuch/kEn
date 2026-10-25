#pragma once

/** @file
 *  @ingroup ken
 */

namespace kEn {

/** @brief Marks a point in the GPU command stream; signaled once every command before it has finished. */
class Fence {
 public:
  Fence()                        = default;
  Fence(const Fence&)            = delete;
  Fence& operator=(const Fence&) = delete;
  Fence(Fence&&)                 = delete;
  Fence& operator=(Fence&&)      = delete;
  virtual ~Fence()               = default;

  /** @brief Whether the commands before the fence have finished; never blocks. */
  [[nodiscard]] virtual bool signaled() = 0;
  /** @brief Blocks until the commands before the fence have finished. */
  virtual void wait() = 0;
};

}  // namespace kEn
