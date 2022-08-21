#include "scene.hpp"


namespace blocks
{
  Scene::Scene()
  {

  }


  bool Scene::ContainsMap()
  {
    return map_ != nullptr;
  }

  std::shared_ptr<Map> Scene::GetMap()
  {
    return map_;
  }


  std::pair<std::vector<std::shared_ptr<ImguiWindow>>::iterator, std::vector<std::shared_ptr<ImguiWindow>>::iterator> Scene::GetImguiWindowsIterator()
  {
    return std::make_pair(imguiWindows_.begin(), imguiWindows_.end());
  }
}
