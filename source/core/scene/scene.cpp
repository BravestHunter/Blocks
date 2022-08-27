#include "scene.hpp"


namespace blocks
{
  Scene::Scene()
  {

  }


  bool Scene::ContainsWorld()
  {
    return world_ != nullptr;
  }

  std::shared_ptr<World> Scene::GetWorld()
  {
    return world_;
  }


  std::pair<std::vector<std::shared_ptr<ImguiWindow>>::iterator, std::vector<std::shared_ptr<ImguiWindow>>::iterator> Scene::GetImguiWindowsIterator()
  {
    return std::make_pair(imguiWindows_.begin(), imguiWindows_.end());
  }
}
