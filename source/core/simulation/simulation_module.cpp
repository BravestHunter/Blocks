#include "simulation_module.hpp"


namespace blocks
{
  SimulationModule::SimulationModule()
  {

  }


  void SimulationModule::Update(float delta, const TimeState& timeState, const InputState& inputState, GameContext& gameContext)
  {
    if (gameContext.scene->ContainsWorld() == false)
    {
      return;
    }

    playerControlModule_.Update(delta, inputState, gameContext);
    entitySpawnModule_.Update(delta, timeState, inputState, gameContext);
    entityAiModule_.Update(delta, timeState, inputState, gameContext);
    physicsModule_.Update(delta, gameContext);
  }


  void SimulationModule::OnSceneChanged(GameContext& gameContext)
  {
    if (gameContext.scene->ContainsWorld())
    {
      gameContext.camera->SetPosition(gameContext.scene->GetWorld()->GetEcsRegistry().get<Transform>(gameContext.scene->GetWorld()->GetPlayerEntity()).position);
    }
  }
}
