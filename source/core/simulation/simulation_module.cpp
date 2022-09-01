#include "simulation_module.hpp"


namespace blocks
{
  SimulationModule::SimulationModule()
  {

  }


  void SimulationModule::Update(float delta, const InputState& inputState, GameContext& gameContext)
  {
    playerControlModule_.Update(delta, inputState, gameContext);
    physicsModule_.Update(delta, gameContext);
  }
}
