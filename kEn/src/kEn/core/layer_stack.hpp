#pragma once

#include <memory>
#include <vector>

#include <kEn/core/layer.hpp>

/** @file
 *  @ingroup ken
 */

namespace kEn {

/**
 * Ordered, ownership-holding container for Layer objects.
 *
 * Layers are stored in a flat vector split into two logical sections:
 *   [0, last_layer_index_)  -- regular layers, iterated front-to-back for update/render.
 *   [last_layer_index_, end) -- overlays, always rendered on top of regular layers.
 *
 * Ownership:
 *   The stack takes exclusive ownership of each layer via unique_ptr. Callers
 *   transfer ownership with push_layer()/push_overlay() and lose access to the
 *   pointer. pop_layer()/pop_overlay() accept a raw observer pointer only to
 *   identify which entry to remove; the removed layer is immediately destroyed.
 *
 * Lifecycle hooks:
 *   on_attach() is called on a layer before it is inserted into the vector.
 *   on_detach() is called before the layer is erased or at stack destruction.
 *
 * Iteration:
 *   Only const iterators are exposed to prevent callers from bypassing the push/pop
 *   API, which would corrupt last_layer_index_.
 *
 * @note Non-copyable and non-movable: ownership transfer has no safe semantics here.
 */
class LayerStack {
 public:
  LayerStack() = default;

  /** Calls on_detach() on every remaining layer in order, then destroys them. */
  ~LayerStack();

  /**
   * Inserts a layer into the regular-layer section (before overlays).
   * on_attach() is called before insertion. Transfers ownership from the caller.
   * @param layer  Owning pointer to the layer to add.
   */
  void push_layer(std::unique_ptr<Layer> layer);

  /**
   * Finds and removes the matching regular layer, calling on_detach() first.
   * @param layer  Observer pointer used only for identity comparison; must not be null.
   *               No-op if the layer is not found in the regular-layer section.
   */
  void pop_layer(Layer* layer);

  /**
   * Appends an overlay to the end of the stack (after all regular layers).
   * on_attach() is called before insertion. Transfers ownership from the caller.
   * @param overlay  Owning pointer to the overlay to add.
   */
  void push_overlay(std::unique_ptr<Layer> overlay);

  /**
   * Finds and removes the matching overlay, calling on_detach() first.
   * @param overlay  Observer pointer used only for identity comparison; must not be null.
   *                 No-op if the overlay is not found in the overlay section.
   */
  void pop_overlay(Layer* overlay);

  /** @return Const iterator to the first layer (front of regular layers). */
  std::vector<std::unique_ptr<Layer>>::const_iterator begin() const { return layers_.begin(); }
  /** @return Const iterator past the last overlay. */
  std::vector<std::unique_ptr<Layer>>::const_iterator end() const { return layers_.end(); }
  /** @return Const reverse iterator starting at the last overlay. */
  std::vector<std::unique_ptr<Layer>>::const_reverse_iterator rbegin() const { return layers_.rbegin(); }
  /** @return Const reverse iterator past the first regular layer. */
  std::vector<std::unique_ptr<Layer>>::const_reverse_iterator rend() const { return layers_.rend(); }

  /** @note Non-copyable and non-movable: owns Layer objects; moving would silently transfer that ownership. */
  LayerStack(const LayerStack&)            = delete;
  LayerStack& operator=(const LayerStack&) = delete;
  LayerStack(LayerStack&&)                 = delete;
  LayerStack& operator=(LayerStack&&)      = delete;

 private:
  std::vector<std::unique_ptr<Layer>> layers_;
  std::size_t last_layer_index_ = 0;
};

}  // namespace kEn
