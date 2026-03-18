#include "layer_stack.hpp"

#include <algorithm>
#include <cstddef>
#include <memory>
#include <utility>

namespace kEn {
LayerStack::~LayerStack() {
  for (auto& layer : layers_) {
    layer->on_detach();
  }
}

void LayerStack::push_layer(std::unique_ptr<Layer> layer) {
  layer->on_attach();
  layers_.emplace(layers_.begin() + static_cast<std::ptrdiff_t>(last_layer_index_), std::move(layer));
  ++last_layer_index_;
}

void LayerStack::push_overlay(std::unique_ptr<Layer> overlay) {
  overlay->on_attach();
  layers_.emplace_back(std::move(overlay));
}

void LayerStack::pop_layer(Layer* layer) {
  const auto end = layers_.begin() + static_cast<std::ptrdiff_t>(last_layer_index_);
  const auto it  = std::find_if(layers_.begin(), end, [layer](const auto& up) { return up.get() == layer; });
  if (it != end) {
    (*it)->on_detach();
    layers_.erase(it);
    --last_layer_index_;
  }
}

void LayerStack::pop_overlay(Layer* overlay) {
  const auto begin = layers_.begin() + static_cast<std::ptrdiff_t>(last_layer_index_);
  const auto it    = std::find_if(begin, layers_.end(), [overlay](const auto& up) { return up.get() == overlay; });
  if (it != layers_.end()) {
    (*it)->on_detach();
    layers_.erase(it);
  }
}
}  // namespace kEn
