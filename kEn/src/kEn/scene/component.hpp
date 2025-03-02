#pragma once

#include <kEn/core/transform.hpp>
#include <kEn/event/event.hpp>

namespace kEn {

class game_object;
class shader;

class game_component {
 public:
  game_component()                                                    = default;
  virtual ~game_component()                                           = default;
  [[nodiscard]] virtual std::shared_ptr<game_component> clone() const = 0;

  virtual void update(float delta)    = 0;
  virtual void render(shader& shader) = 0;
  virtual void on_event(base_event& event) { dispatcher_.dispatch(event); }

  const game_object& parent() const { return parent_.value(); }
  game_object& parent() { return parent_.value(); }

  const kEn::transform& transform() const;
  kEn::transform& transform();

  VIRTUAL_FIVE(game_component);

 protected:
  std::optional<std::reference_wrapper<game_object>> parent_;
  kEn::event_dispatcher dispatcher_;
  friend game_object;
};

}  // namespace kEn
