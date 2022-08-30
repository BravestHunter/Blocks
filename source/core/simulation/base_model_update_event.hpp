#pragma once

#include "model_update_event_type.hpp"


namespace blocks
{
  class BaseModelUpdateEvent
  {
  public:
    BaseModelUpdateEvent(ModelUpdateEventType type) : type_(type) {}

    inline ModelUpdateEventType GetType() const;

  private:
    ModelUpdateEventType type_;
  };


  ModelUpdateEventType BaseModelUpdateEvent::GetType() const
  {
    return type_;
  }
}
