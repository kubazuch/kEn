#include "model_component.hpp"

#include <memory>

#include <kEn/core/assert.hpp>
#include <kEn/renderer/shader.hpp>

namespace kEn {

void ModelComponent::render(Shader& shader, double /*alpha*/) {
  KEN_CORE_ASSERT(has_parent(), "Can't render parentless model!");
  model_->render(shader, transform());
}

std::unique_ptr<GameComponent> ModelComponent::clone() const { return std::make_unique<ModelComponent>(model_); }

}  // namespace kEn
