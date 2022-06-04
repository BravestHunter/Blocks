#pragma once

#include <memory>
#include <mutex>

#include "export.h"
#include "environment.hpp"
#include "render/opengl_render_module.hpp"


#include "render/opengl_render_system.hpp"
#include "resource/resource_base.hpp"
#include "render/opengl_scene.hpp"
#include "camera.hpp"
#include "scene/map.hpp"
#include "scene/scene.hpp"
#include "collision/aabb.hpp"


class DllExport Game
{
public:
  Game(int width, int height);
  Game(const Game&) = delete;
  Game(Game&& other) = delete;
  Game& operator=(const Game&) = delete;
  Game& operator=(Game&& other) = delete;
  ~Game();

  int Run();

private:
  bool isRunning_ = true;

  // Game modules
  OpenglRenderModule renderModule_;

  ResourceBase resourceBase_;

  // Window
  int framebufferWidth_;
  int framebufferHeight_;
  bool isCursorEnabled_ = true;

  // Input
  bool isWPressed_ = false;
  bool isSPressed_ = false;
  bool isAPressed_ = false;
  bool isDPressed_ = false;
  float lastMouseX_;
  float lastMouseY_;
  bool firstMouse_ = true;

  // Camera
  std::shared_ptr<Camera> camera_;

  // Collision
  AABB playerBounds_ = AABB(glm::vec3(-0.25f, -0.25f, -0.25f), glm::vec3(0.25f, 0.25f, 0.25f));

  //Chunks
  int renderRadius_ = 3;
  glm::ivec2 lastCenterChunkCoords_;

  glm::ivec2 CalculateChunkCenter();
  void RunSimulationCycle();
  void AddChunks(glm::ivec2 centerChunkCoords);
  void RemoveChunks(glm::ivec2 centerChunkCoords, glm::ivec2 lastCenterChunkCoords);
  void RunRenderCycle();
  void ProcessInput(GlfwWindow& window);
  void SwitchCursorMode(GlfwWindow& window);
  void RunFixedUpdateCycle();
  void MovePlayer();

  std::shared_ptr<Scene> currentScene_ = nullptr;
  std::shared_ptr<Scene> requestedScene_ = nullptr;
  std::mutex sceneMutex_;
  void RequestScene(std::shared_ptr<Scene> scene);
  void SetRequestedScene();
  std::shared_ptr<Scene> CreateMainMenuScene();
  std::shared_ptr<Scene> CreateWorldScene(std::shared_ptr<Map> map);
  std::shared_ptr<Map> LoadMap();
  void SaveMap(std::shared_ptr<Map> map);
};
