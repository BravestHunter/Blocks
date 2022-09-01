#pragma once

#include "game_context.hpp"


namespace blocks
{
  class PhysicsModule
  {
  public:
    void Update(float delta, GameContext& gameContext);
  };
}
