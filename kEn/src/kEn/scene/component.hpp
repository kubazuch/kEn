#pragma once

#include <memory>

#include <kEn/core/assert.hpp>
#include <kEn/core/timestep.hpp>
#include <kEn/core/transform.hpp>
#include <kEn/event/event.hpp>

namespace kEn {

class GameObject;
class Shader;

class GameComponent {
 public:
  GameComponent()          = default;
  virtual ~GameComponent() = default;

  [[nodiscard]] virtual std::unique_ptr<GameComponent> clone() const = 0;

  virtual void on_attach() {}
  virtual void on_detach() {}

  virtual void update(Timestep, Timestep) {}
  virtual void render(Shader&, double) {}
  virtual void imgui() {}
  bool on_event(BaseEvent& event) { return dispatcher_.dispatch(event); }

  [[nodiscard]] bool has_parent() const noexcept { return parent_ != nullptr; }

  [[nodiscard]] const GameObject& parent() const {
    KEN_CORE_ASSERT(parent_ != nullptr);
    return *parent_;
  }
  [[nodiscard]] GameObject& parent() {
    KEN_CORE_ASSERT(parent_ != nullptr);
    return *parent_;
  }

  [[nodiscard]] const kEn::Transform& transform() const;
  [[nodiscard]] kEn::Transform& transform();

  DELETE_COPY_MOVE(GameComponent);

 protected:
  kEn::EventDispatcher dispatcher_;

 private:
  void attach_to(GameObject& go);
  void detach_from_parent() noexcept;

  GameObject* parent_ = nullptr;

  friend GameObject;
};

}  // namespace kEn
