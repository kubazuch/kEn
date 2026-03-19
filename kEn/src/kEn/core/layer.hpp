#pragma once

#include <chrono>
#include <string>
#include <string_view>

#include <kEn/event/event.hpp>

/** @file
 *  @ingroup ken
 */

namespace kEn {

/** Tick/frame delta type used throughout the engine's update pipeline. */
using duration_t = std::chrono::nanoseconds;

/**
 * Abstract base class for all engine layers.
 *
 * Layers form an ordered pipeline inside LayerStack. The Application drives them in
 * insertion order for update/render and in reverse order for event dispatch.
 *
 * Lifecycle:
 *   on_attach()  -- called once when the layer is pushed onto the stack.
 *   on_detach()  -- called once when the layer is popped or the stack is destroyed.
 *   on_update()  -- called every fixed tick (60 Hz by default) with delta and elapsed time.
 *   on_render()  -- called every frame; alpha is the interpolation factor within the current tick.
 *   on_imgui()   -- called every frame inside an ImguiFrame scope; draw Dear ImGui widgets here.
 *   on_event()   -- called for each event in reverse stack order; return true to consume it.
 *
 * Derived classes override only the hooks they need; all defaults are no-ops.
 */
class Layer {
 public:
  /** Constructs the layer with an optional debug name shown in tooling. */
  explicit Layer(std::string_view name = "Layer");
  virtual ~Layer() = default;

  /** Called once after the layer is pushed onto the LayerStack. Allocate GPU resources here. */
  virtual void on_attach() {}

  /** Called once before the layer is removed from the LayerStack. Release GPU resources here. */
  virtual void on_detach() {}

  /**
   * Called every fixed tick.
   * @p delta is the time elapsed since the previous tick; @p time is total elapsed since startup.
   */
  virtual void on_update(duration_t /*delta*/, duration_t /*time*/) {}

  /**
   * Called every frame for rendering.
   * @p alpha is the interpolation factor [0, 1) between the previous and current tick state.
   */
  virtual void on_render(double /*alpha*/) {}

  /** Called every frame inside an active ImguiFrame scope. Submit Dear ImGui calls here. */
  virtual void on_imgui() {}

  /**
   * Called for each event propagated down the LayerStack (reverse insertion order).
   * @return true to mark the event as handled and stop further propagation; false to pass it on.
   */
  virtual bool on_event(BaseEvent& /*event*/) { return false; }

  /** @return The debug name supplied at construction. */
  [[nodiscard]] const std::string& name() const { return debug_name_; }

  VIRTUAL_FIVE(Layer);

 private:
  std::string debug_name_;
};

}  // namespace kEn
