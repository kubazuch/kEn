#pragma once

#include <kEn/core/layer.hpp>

namespace kEn {

class LayerStack {
 public:
  LayerStack() = default;
  ~LayerStack();

  void push_layer(Layer* layer);
  void pop_layer(Layer* layer);
  void push_overlay(Layer* overlay);
  void pop_overlay(Layer* overlay);

  std::vector<Layer*>::iterator begin() { return layers_.begin(); }
  std::vector<Layer*>::iterator end() { return layers_.end(); }
  std::vector<Layer*>::reverse_iterator rbegin() { return layers_.rbegin(); }
  std::vector<Layer*>::reverse_iterator rend() { return layers_.rend(); }

  std::vector<Layer*>::const_iterator begin() const { return layers_.begin(); }
  std::vector<Layer*>::const_iterator end() const { return layers_.end(); }
  std::vector<Layer*>::const_reverse_iterator rbegin() const { return layers_.rbegin(); }
  std::vector<Layer*>::const_reverse_iterator rend() const { return layers_.rend(); }

  VIRTUAL_FIVE(LayerStack);

 private:
  std::vector<Layer*> layers_;
  unsigned int last_layer_index_ = 0;
};

}  // namespace kEn
