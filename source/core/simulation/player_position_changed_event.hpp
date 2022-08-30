#pragma once

#include <glm/vec3.hpp>

#include "base_model_update_event.hpp"
#include "model_update_event_type.hpp"


namespace blocks
{
  class PlayerPositionChangedEvent : public BaseModelUpdateEvent
  {
  public:
    PlayerPositionChangedEvent(glm::vec3 position) : BaseModelUpdateEvent(ModelUpdateEventType::PlayerPositionChanged), position_(position) {}

    inline glm::vec3 GetPosition() const;

  private:
    glm::vec3 position_;
  };


  glm::vec3 PlayerPositionChangedEvent::GetPosition() const
  {
    return position_;
  }
}
