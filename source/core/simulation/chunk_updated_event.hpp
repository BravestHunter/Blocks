#pragma once

#include <glm/vec3.hpp>

#include "chunk_position.hpp"

#include "base_model_update_event.hpp"
#include "model_update_event_type.hpp"


namespace blocks
{
  class ChunkUpdatedEvent : public BaseModelUpdateEvent
  {
  public:
    ChunkUpdatedEvent(ChunkPosition position) : BaseModelUpdateEvent(ModelUpdateEventType::ChunkUpdated), position_(position) {}

    inline ChunkPosition GetPosition() const;

  private:
    ChunkPosition position_;
  };


  ChunkPosition ChunkUpdatedEvent::GetPosition() const
  {
    return position_;
  }
}
