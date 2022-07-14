#pragma once

#include <memory>

#include "camera.hpp"
#include "scene/scene.hpp"
#include "geometry/aabb.hpp"
#include "render/opengl_scene.hpp"

namespace blocks
{
  struct GameContext
  {
    std::shared_ptr<Camera> camera;
    std::shared_ptr<Scene> scene;
    std::shared_ptr<OpenglScene> openglScene;

    bool isCursorEnabled = true;

    blocks::AABB playerBounds;
  };
}
