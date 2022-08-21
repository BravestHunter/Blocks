#pragma once

#include <vector>
#include <memory>
#include <utility>

#include "map.hpp"

#include "ui/imgui_window.hpp"


namespace blocks
{
  class SceneBuilder;

  class Scene
  {
    friend SceneBuilder;

  public:
    Scene();

    bool ContainsMap();
    std::shared_ptr<Map> GetMap();

    std::pair<std::vector<std::shared_ptr<ImguiWindow>>::iterator, std::vector<std::shared_ptr<ImguiWindow>>::iterator> GetImguiWindowsIterator();

  private:
    std::shared_ptr<Map> map_ = nullptr;
    std::vector<std::shared_ptr<ImguiWindow>> imguiWindows_;
  };
}
