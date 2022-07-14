#pragma once

#include <memory>
#include <mutex>

#include "export.h"
#include "environment.hpp"
#include "game_context.hpp"
#include "input/input_module.hpp"
#include "render/opengl_render_module.hpp"
#include "player_control_module.hpp"
#include "map_loading_module.hpp"


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

  private:
    void RunSimulationCycle();
    void RunRenderCycle();

    void SwitchCursorMode(GlfwWindow& window);

    std::shared_ptr<Scene> requestedScene_ = nullptr;
    std::mutex sceneMutex_;
    void RequestScene(std::shared_ptr<Scene> scene);
    void SetRequestedScene();
    std::shared_ptr<Scene> CreateMainMenuScene();
    std::shared_ptr<Scene> CreateWorldScene(std::shared_ptr<Map> map);

    bool isRunning_ = true;

    GlfwWindow window_;
    GameContext context_;

    // Game modules
    InputModule inputModule_;
    OpenglRenderModule renderModule_;
    PlayerControlModule playerControlModule_;
    MapLoadingModule mapLoadingModule_;
  };
}
