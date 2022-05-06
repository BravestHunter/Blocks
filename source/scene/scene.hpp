#pragma once

#include <vector>
#include <memory>
#include <utility>

#include "map.hpp"

#include "ui/imgui_window.hpp"


class Scene
{
public:
  Scene();

  void AddImguiWindow(std::shared_ptr<ImguiWindow> window);

  bool ContainsMap();
  void SetMap(std::shared_ptr<Map> map);
  std::shared_ptr<Map> GetMap();

  std::pair<std::vector<std::shared_ptr<ImguiWindow>>::iterator, std::vector<std::shared_ptr<ImguiWindow>>::iterator> GetImguiWindowsIterator();

private:
  std::shared_ptr<Map> map_ = nullptr;
  std::vector<std::shared_ptr<ImguiWindow>> imguiWindows_;
};
