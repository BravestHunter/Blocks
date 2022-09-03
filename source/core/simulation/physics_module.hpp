#pragma once

#include "game_context.hpp"


namespace blocks
{
  class PhysicsModule
  {
  public:
    void Update(float delta, GameContext& gameContext);

  private:
    bool ProcessPlayerVelocity(float delta, Entity& player, glm::vec3 velocity, GameContext& gameContext);
    bool CollidesMap(AABB& aabb, std::shared_ptr<Map> map);
  };
}
