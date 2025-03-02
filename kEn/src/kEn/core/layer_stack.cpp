#include <kEn/core/layer_stack.hpp>
#include <kenpch.hpp>

namespace kEn {
layer_stack::~layer_stack() {
  for (layer* layer : layers_) {
    layer->on_detach();
    delete layer;
  }
}

void layer_stack::push_layer(layer* layer) {
  layers_.emplace(layers_.begin() + last_layer_index_, layer);
  ++last_layer_index_;
  layer->on_attach();
}

void layer_stack::push_overlay(layer* overlay) {
  layers_.emplace_back(overlay);
  overlay->on_attach();
}

void layer_stack::pop_layer(layer* layer) {
  const auto it = std::find(layers_.begin(), layers_.begin() + last_layer_index_, layer);
  if (it != layers_.begin() + last_layer_index_) {
    layer->on_detach();
    layers_.erase(it);
    --last_layer_index_;
  }
}

void layer_stack::pop_overlay(layer* overlay) {
  const auto it = std::find(layers_.begin() + last_layer_index_, layers_.end(), overlay);
  if (it != layers_.end()) {
    overlay->on_detach();
    layers_.erase(it);
  }
}
}  // namespace kEn
