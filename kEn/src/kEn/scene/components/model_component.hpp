#pragma once

#include <memory>

#include <kEn/imgui/editors/model.hpp>
#include <kEn/scene/assets/model.hpp>
#include <kEn/scene/component.hpp>

/**
 * @file
 * @ingroup ken
 */

namespace kEn {

/**
 * @brief Renders a @ref Model asset as a @ref GameComponent.
 *
 * Holds a shared reference to a @ref Model and submits it to the renderer each
 * frame via @ref render.  Multiple @c ModelComponent instances may share the
 * same underlying @ref Model.
 */
class ModelComponent : public GameComponent {
 public:
  /** @brief Construct with the given model asset. */
  explicit ModelComponent(std::shared_ptr<Model> model) : model_(std::move(model)) {}

  void render(Shader& shader, double alpha) override;
  void imgui() override { ui::Model(*model_); }
  [[nodiscard]] std::unique_ptr<GameComponent> clone() const override;

 private:
  std::shared_ptr<Model> model_;
};

}  // namespace kEn
