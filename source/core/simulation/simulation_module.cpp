#include "simulation_module.hpp"


namespace blocks
{
  SimulationModule::SimulationModule()
  {

  }


  void SimulationModule::Update(float delta, const InputState& inputState, GameContext& gameContext)
  {
    if (gameContext.scene->ContainsWorld() == false)
    {
      return;
    }

    playerControlModule_.Update(delta, inputState, gameContext);
    physicsModule_.Update(delta, gameContext);
  }
}
