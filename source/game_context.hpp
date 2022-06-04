#pragma once

#include <memory>

#include "camera.hpp"
#include "scene/scene.hpp"
#include "collision/aabb.hpp"


struct GameContext
{
  std::shared_ptr<Camera> camera;
  std::shared_ptr<Scene> scene;

  bool isCursorEnabled = true;

  // Input
  bool isWPressed = false;
  bool isSPressed = false;
  bool isAPressed = false;
  bool isDPressed = false;
  bool firstMouse = true;
  float lastMouseX;
  float lastMouseY;

  AABB playerBounds;
};
