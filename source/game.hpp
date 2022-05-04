#pragma once

#include "export.h"

#include "platform/glfw_platform.hpp"
#include "render/opengl_render_system.hpp"
#include "render/opengl_scene.hpp"
#include "camera.hpp"
#include "scene/map.hpp"


class DllExport Game
{
public:
  Game(int width, int height);
  ~Game();

  int Run();

private:
  std::unique_ptr<GlfwPlatform> platform_;
  std::unique_ptr<Map> map_;
  std::shared_ptr<OpenglScene> openglScene_;

  int renderRadius_ = 3;
  glm::ivec2 lastCenterChunkCoords_;

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

  // Window
  std::unique_ptr<GlfwWindow> window_;
  bool isCursorEnabled_ = false;

  glm::ivec2 CalculateChunkCenter();
  void RunSimulationCycle();
  void AddChunks(glm::ivec2 centerChunkCoords);
  void RemoveChunks(glm::ivec2 centerChunkCoords, glm::ivec2 lastCenterChunkCoords);
  void RunRenderCycle();
  void ProcessInput();
  void SwitchCursorMode();
};
