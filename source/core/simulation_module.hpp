#pragma once

#include <memory>

#include <glm/glm.hpp>

#include "world.hpp"
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

    void SetWorld(std::shared_ptr<World> world);
    std::shared_ptr<World> GetWorld() const;

    void SetRenderModule(OpenglRenderModule* renderModule);
    void ProcessChunksToAdd(float delta, GameContext& context);
    void OnSceneChanged(GameContext& context);

  private:
    PlayerControlModule playerControlModule_;
    MapLoadingModule mapLoadingModule_;

    std::shared_ptr<World> world_ = nullptr;
  };
}
