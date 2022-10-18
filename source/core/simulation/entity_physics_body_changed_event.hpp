#pragma once

#include <glm/vec3.hpp>
#include "entt/entt.hpp"

#include "ecs/components/transform.hpp"
#include "ecs/components/physics_body.hpp"
#include "base_model_update_event.hpp"
#include "model_update_event_type.hpp"


namespace blocks
{
  class EntityPhysicsBodyChangedEvent : public BaseModelUpdateEvent
  {
  public:
    EntityPhysicsBodyChangedEvent(const entt::entity& entity, const PhysicsBody& physicsBody, const Transform& transform)
      : BaseModelUpdateEvent(ModelUpdateEventType::EntityPhysicsBodyChanged), entity_(entity), bounds_(transform.position + physicsBody.bounds.center, physicsBody.bounds.size)
    {
    }

    inline const entt::entity& GetEntity() const;
    inline const AABB& GetBounds() const;

  private:
    entt::entity entity_;
    AABB bounds_;
  };


  const entt::entity& EntityPhysicsBodyChangedEvent::GetEntity() const
  {
    return entity_;
  }

  const AABB& EntityPhysicsBodyChangedEvent::GetBounds() const
  {
    return bounds_;
  }
}
