#pragma once

#include <memory>

#include <glm/glm.hpp>

#include "game_context.hpp"
#include "player_control_module.hpp"
#include "map_loading_module.hpp"


namespace blocks
{
  class SimulationModule
  {
  public:
    SimulationModule();

    void Update(float delta, const InputState& inputState, GameContext& context);

    void SetRenderModule(OpenglRenderModule* renderModule);
    void ProcessChunksToAdd(float delta, GameContext& context);
    void OnSceneChanged(GameContext& context);

  private:
    PlayerControlModule playerControlModule_;
    MapLoadingModule mapLoadingModule_;
  };
}
