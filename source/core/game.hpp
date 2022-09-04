#pragma once

#include <memory>
#include <mutex>

#include "export.h"
#include "environment.hpp"
#include "game_context.hpp"
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
    void RequestScene(std::shared_ptr<Scene> scene);

    GameContext& GetContext();

  private:
    void RunSimulationCycle();
    void RunPresentationCycle();
    void RunPresentationUpdateCycle();

    void SwitchCursorMode(GlfwWindow& window);
    void Stop();
    void SetRequestedScene();

    std::shared_ptr<Scene> requestedScene_ = nullptr;
    std::mutex sceneMutex_;

    bool isRunning_ = true;

    GlfwWindow window_;
    GameContext context_;

    // Game modules
    InputModule inputModule_;
    SimulationModule simulationModule_;
    PresentationModule presentationModule_;
  };
}
