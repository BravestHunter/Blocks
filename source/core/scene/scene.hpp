#pragma once

#include <vector>
#include <memory>
#include <utility>
#include <functional>

#include "world.hpp"

#include "ui/imgui_window.hpp"


namespace blocks
{
  class SceneBuilder;

  class Scene
  {
    friend SceneBuilder;

  public:
    Scene(bool menuModeOnly);

    void OnMenuModeChanged(bool isMenuMode);

    inline bool MenuModeOnlyAllowed() const;
    bool ContainsWorld();
    std::shared_ptr<World> GetWorld();

    std::pair<std::vector<std::shared_ptr<ImguiWindow>>::iterator, std::vector<std::shared_ptr<ImguiWindow>>::iterator> GetImguiWindowsIterator();

  private:
    bool menuModeOnly_;
    std::shared_ptr<World> world_ = nullptr;
    std::vector<std::shared_ptr<ImguiWindow>> imguiWindows_;
    std::function<void(bool)> menuModeCallbackFunction_ = nullptr;
  };



  bool Scene::MenuModeOnlyAllowed() const
  {
    return menuModeOnly_;
  }
}
