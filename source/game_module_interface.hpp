#pragma once

#include "game_context.hpp"


namespace blocks
{
  class GameModuleInterface
  {
  public:
    virtual ~GameModuleInterface() {};

    virtual void Update(float delta, GameContext& context) = 0;
  };
}
