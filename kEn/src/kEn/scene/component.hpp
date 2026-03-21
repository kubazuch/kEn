#pragma once

#include <memory>

#include <kEn/core/timestep.hpp>
#include <kEn/core/transform.hpp>
#include <kEn/event/event.hpp>

namespace kEn {

class GameObject;
class Shader;

class GameComponent {
 public:
  GameComponent() = default;
  virtual ~GameComponent() { on_detach(); }
  [[nodiscard]] virtual std::shared_ptr<GameComponent> clone() const = 0;

  virtual void on_attach() {}
  virtual void on_detach() {}

  virtual void update(Timestep delta, Timestep time) = 0;
  virtual void render(Shader& shader, double alpha)  = 0;
  virtual void imgui()                               = 0;
  virtual bool on_event(BaseEvent& event) { return dispatcher_.dispatch(event); }
  virtual void on_transform_changed() = 0;

  const GameObject& parent() const { return parent_.value(); }
  GameObject& parent() { return parent_.value(); }

  const kEn::Transform& transform() const;
  kEn::Transform& transform();

  DELETE_COPY_MOVE(GameComponent);

 protected:
  std::optional<std::reference_wrapper<GameObject>> parent_;
  kEn::EventDispatcher dispatcher_;
  friend GameObject;
};

}  // namespace kEn
