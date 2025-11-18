#include "layer_stack.hpp"

#include <kEn/core/layer.hpp>

namespace kEn {
LayerStack::~LayerStack() {
  for (Layer* layer : layers_) {
    layer->on_detach();
    delete layer;  // NOLINT
  }
}

void LayerStack::push_layer(Layer* layer) {
  layers_.emplace(layers_.begin() + last_layer_index_, layer);
  ++last_layer_index_;
  layer->on_attach();
}

void LayerStack::push_overlay(Layer* overlay) {
  layers_.emplace_back(overlay);
  overlay->on_attach();
}

void LayerStack::pop_layer(Layer* layer) {
  const auto it = std::find(layers_.begin(), layers_.begin() + last_layer_index_, layer);
  if (it != layers_.begin() + last_layer_index_) {
    layer->on_detach();
    layers_.erase(it);
    --last_layer_index_;
  }
}

void LayerStack::pop_overlay(Layer* overlay) {
  const auto it = std::find(layers_.begin() + last_layer_index_, layers_.end(), overlay);
  if (it != layers_.end()) {
    overlay->on_detach();
    layers_.erase(it);
  }
}
}  // namespace kEn
