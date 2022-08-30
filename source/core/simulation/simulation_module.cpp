#include "simulation_module.hpp"


namespace blocks
{
  SimulationModule::SimulationModule()
  {

  }


  void SimulationModule::Update(float delta, const InputState& inputState, GameContext& context)
  {
    playerControlModule_.Update(delta, inputState, context);
  }
}
