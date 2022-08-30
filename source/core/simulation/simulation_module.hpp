#pragma once

#include <memory>

#include <glm/glm.hpp>

#include "game_context.hpp"
#include "player_control_module.hpp"


namespace blocks
{
  class SimulationModule
  {
  public:
    SimulationModule();

    void Update(float delta, const InputState& inputState, GameContext& context);

  private:
    PlayerControlModule playerControlModule_;
  };
}
