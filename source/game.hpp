#pragma once

#include "export.h"

#include "platform/glfw_platform.hpp"
#include "camera.hpp"


class DllExport Game
{
public:
  Game(int width, int height);
  ~Game();

  int Run();

private:
  std::unique_ptr<GlfwPlatform> platform_;

  int framebufferWidth_;
  int framebufferHeight_;

  // Camera
  std::unique_ptr<Camera> camera_;
  float lastX;
  float lastY;
  bool firstMouse = true;

  void processInput(std::shared_ptr<GlfwWindow> window);
};
