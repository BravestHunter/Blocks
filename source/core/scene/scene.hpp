#pragma once

#include <vector>
#include <memory>
#include <utility>

#include "world.hpp"

#include "ui/imgui_window.hpp"


namespace blocks
{
  class SceneBuilder;

  class Scene
  {
    friend SceneBuilder;

  public:
    Scene();

    bool ContainsWorld();
    std::shared_ptr<World> GetWorld();

    std::pair<std::vector<std::shared_ptr<ImguiWindow>>::iterator, std::vector<std::shared_ptr<ImguiWindow>>::iterator> GetImguiWindowsIterator();

  private:
    std::shared_ptr<World> world_ = nullptr;
    std::vector<std::shared_ptr<ImguiWindow>> imguiWindows_;
  };
}
