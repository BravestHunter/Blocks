#include "simulation_module.hpp"


namespace blocks
{
  SimulationModule::SimulationModule()
  {

  }


  void SimulationModule::Update(float delta, const InputState& inputState, GameContext& context)
  {
    playerControlModule_.Update(delta, inputState, context);
    mapLoadingModule_.Update(delta, context);
  }


  void SimulationModule::SetRenderModule(OpenglRenderModule* renderModule)
  {
    mapLoadingModule_.SetRenderModule(renderModule);
  }

  void SimulationModule::ProcessChunksToAdd(float delta, GameContext& context)
  {
    mapLoadingModule_.ProcessChunksToAdd(delta, context);
  }

  void SimulationModule::OnSceneChanged(GameContext& context)
  {
    mapLoadingModule_.OnSceneChanged(context);
  }
}
