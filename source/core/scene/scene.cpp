#include "scene.hpp"


namespace blocks
{
  Scene::Scene()
  {

  }


  void Scene::AddImguiWindow(std::shared_ptr<ImguiWindow> window)
  {
    imguiWindows_.push_back(window);
  }


  bool Scene::ContainsMap()
  {
    return map_ != nullptr;
  }

  void Scene::SetMap(std::shared_ptr<Map> map)
  {
    map_ = map;
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
