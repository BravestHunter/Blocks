#pragma once

#include <memory>

#include <glm/glm.hpp>

#include "game_context.hpp"
#include "time/time_state.hpp"
#include "player_control_module.hpp"
#include "entity_spawn_module.hpp"
#include "entity_ai_module.hpp"
#include "physics_module.hpp"


namespace blocks
{
  class SimulationModule
  {
  public:
    SimulationModule();

    void Update(float delta, const TimeState& timeState, const InputState& inputState, GameContext& gameContext);

    void OnSceneChanged(GameContext& gameContext);

  private:
    PlayerControlModule playerControlModule_;
    EntitySpawnModule entitySpawnModule_;
    EntityAiModule entityAiModule_;
    PhysicsModule physicsModule_;
  };
}
