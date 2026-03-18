#pragma once

#include <memory>
#include <vector>

#include <kEn/core/layer.hpp>

namespace kEn {

class LayerStack {
 public:
  LayerStack() = default;
  ~LayerStack();

  void push_layer(std::unique_ptr<Layer> layer);
  void pop_layer(Layer* layer);
  void push_overlay(std::unique_ptr<Layer> overlay);
  void pop_overlay(Layer* overlay);

  std::vector<std::unique_ptr<Layer>>::const_iterator begin() const { return layers_.begin(); }
  std::vector<std::unique_ptr<Layer>>::const_iterator end() const { return layers_.end(); }
  std::vector<std::unique_ptr<Layer>>::const_reverse_iterator rbegin() const { return layers_.rbegin(); }
  std::vector<std::unique_ptr<Layer>>::const_reverse_iterator rend() const { return layers_.rend(); }

  // Non-copyable and non-movable: owns Layer objects; moving would silently transfer that ownership
  LayerStack(const LayerStack&)            = delete;
  LayerStack& operator=(const LayerStack&) = delete;
  LayerStack(LayerStack&&)                 = delete;
  LayerStack& operator=(LayerStack&&)      = delete;

 private:
  std::vector<std::unique_ptr<Layer>> layers_;
  std::size_t last_layer_index_ = 0;
};

}  // namespace kEn
