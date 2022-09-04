#pragma once

#include <memory>

#include <glm/glm.hpp>

#include "game_context.hpp"
#include "player_control_module.hpp"
#include "physics_module.hpp"


namespace blocks
{
  class SimulationModule
  {
  public:
    SimulationModule();

    void Update(float delta, const InputState& inputState, GameContext& gameContext);

    void OnSceneChanged(GameContext& gameContext);

  private:
    PlayerControlModule playerControlModule_;
    PhysicsModule physicsModule_;
  };
}
