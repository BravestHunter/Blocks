#pragma once

#include <memory>
#include <utility>

#include "opengl_map.hpp"
#include "chunk.hpp"


namespace blocks
{
  class OpenglRenderSystem;

  class OpenglScene
  {
    friend OpenglRenderSystem;

  public:
    OpenglScene();
    ~OpenglScene();

    void InitMap();

    std::shared_ptr<OpenglMap> GetMap();

  private:
    std::shared_ptr<OpenglMap> map_;
  };
}
