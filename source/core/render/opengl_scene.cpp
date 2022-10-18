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
}
