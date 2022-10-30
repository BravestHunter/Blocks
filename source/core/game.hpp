#pragma once

#include <memory>
#include <mutex>

#include "export.h"
#include "environment.hpp"
#include "game_context.hpp"
#include "time/time_module.hpp"
#include "input/input_module.hpp"
#include "simulation/simulation_module.hpp"
#include "presentation/presentation_module.hpp"


namespace blocks
{
  class DllExport Game
  {
  public:
    Game(int width, int height);
    Game(const Game&) = delete;
    Game(Game&& other) = delete;
    Game& operator=(const Game&) = delete;
    Game& operator=(Game&& other) = delete;
    ~Game();

    int Run();
    void Stop();

    void RequestScene(std::shared_ptr<Scene> scene);
    void SetMenuMode(bool isMenuMode);

    GameContext& GetContext();
    inline const TimeState& GetTimeState() const;

  private:
    void RunSimulationCycle();
    void RunPresentationCycle();
    void RunPresentationUpdateCycle();

    void SetRequestedScene();

    std::shared_ptr<Scene> requestedScene_ = nullptr;
    std::mutex sceneMutex_;

    bool isRunning_ = true;

    GlfwWindow window_;
    GameContext context_;

    // Game modules
    TimeModule timeModule_;
    InputModule inputModule_;
    SimulationModule simulationModule_;
    PresentationModule presentationModule_;
  };


  const TimeState& Game::GetTimeState() const
  {
    return timeModule_.GetState();
  }
}
