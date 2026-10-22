#pragma once

#include <cstddef>

#include <kEn/core/core.hpp>

namespace kEn::profile {

/** @brief Creates the profiler's GPU context; the OpenGL context must be current. No-op without TRACY_ENABLE. */
void create_gpu_context();

/**
 * @brief Reads back finished GPU timer queries and plots the frame's draw calls and uploaded bytes; called once per
 * frame after the swap. No-op without TRACY_ENABLE.
 */
void end_frame();

#ifdef TRACY_ENABLE
/** @brief Counts one draw call towards the current frame. */
void count_draw();

/** @brief Counts bytes sent from the CPU to a buffer or texture towards the current frame. */
void count_upload(std::size_t bytes);
#else
inline void count_draw() {}
inline void count_upload(std::size_t /*bytes*/) {}
#endif

}  // namespace kEn::profile
