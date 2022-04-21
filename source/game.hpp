#pragma once

#include "export.h"

#include "platform/glfw_platform.hpp"
#include "render/opengl_render_system.hpp"
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

  // Timing
  float deltaTime_ = 0.0f;	// time between current frame and last frame
  float lastFrame_ = 0.0f;

  // Camera
  std::unique_ptr<Camera> camera_;
  float lastX_;
  float lastY_;
  bool firstMouse_ = true;

  void processInput(std::shared_ptr<GlfwWindow> window);
};
