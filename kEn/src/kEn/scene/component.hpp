#pragma once

#include <kEn/core/transform.hpp>
#include <kEn/event/event.hpp>

namespace kEn {

class GameObject;
class Shader;

class GameComponent {
 public:
  GameComponent()                                                    = default;
  virtual ~GameComponent()                                           = default;
  [[nodiscard]] virtual std::shared_ptr<GameComponent> clone() const = 0;

  virtual void update(float delta)    = 0;
  virtual void render(Shader& shader) = 0;
  virtual void on_event(BaseEvent& event) { dispatcher_.dispatch(event); }

  const GameObject& parent() const { return parent_.value(); }
  GameObject& parent() { return parent_.value(); }

  const kEn::Transform& transform() const;
  kEn::Transform& transform();

  VIRTUAL_FIVE(GameComponent);

 protected:
  std::optional<std::reference_wrapper<GameObject>> parent_;
  kEn::EventDispatcher dispatcher_;
  friend GameObject;
};

}  // namespace kEn
