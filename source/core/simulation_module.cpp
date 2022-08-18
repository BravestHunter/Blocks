#include "simulation_module.hpp"


namespace blocks
{
  SimulationModule::SimulationModule()
  {

  }


  void SimulationModule::Update(float delta, const InputState& inputState, GameContext& context)
  {
    //if (world_ == nullptr)
    //{
    //  return;
    //}

    playerControlModule_.Update(delta, inputState, context);
    mapLoadingModule_.Update(delta, context);
  }


  void SimulationModule::SetWorld(std::shared_ptr<World> world)
  {
    world_ = world;
  }

  std::shared_ptr<World> SimulationModule::GetWorld() const
  {
    return world_;
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
