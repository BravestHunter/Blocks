#pragma once

#include "game_context.hpp"
#include "ecs/components/transform.hpp"
#include "ecs/components/physics_body.hpp"


namespace blocks
{
  class PhysicsModule
  {
  public:
    void Update(float delta, GameContext& gameContext);

  private:
    bool ProcessVelocity(float delta, Transform& transform, PhysicsBody& physicsBody, glm::vec3 velocity, GameContext& gameContext);
    bool CollidesMap(AABB& aabb, std::shared_ptr<Map> map);
  };
}
