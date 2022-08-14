#include "opengl_scene.hpp"

#include <exception>


namespace blocks
{
  OpenglScene::OpenglScene()
  {

  }

  OpenglScene::~OpenglScene()
  {

  }


  void OpenglScene::InitMap()
  {
    map_ = std::make_unique<OpenglMap>();
  }


  std::shared_ptr<OpenglMap> OpenglScene::GetMap()
  {
    return map_;
  }
}
