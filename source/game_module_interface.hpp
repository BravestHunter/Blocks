#pragma once

#include "game_context.hpp"


class GameModuleInterface
{
public:
  virtual ~GameModuleInterface() {};

  virtual void Update(float delta, GameContext& context) = 0;
};
