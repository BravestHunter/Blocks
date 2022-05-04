#include "game.hpp"

#include <memory>
#include <thread>
#include <string>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "imgui_internal.h"

#include "model/chunk.hpp"
#include "render/opengl_model.hpp"
#include "render/opengl_chunk.hpp"
#include "render/opengl_texture_array.hpp"

#include "compile_utils.hpp"
#include "resourceConfig.h"

#include "io/file_api.hpp"


Game::Game(int width, int height) : framebufferWidth_(width), framebufferHeight_(height), lastX_(width / 2), lastY_(height / 2)
{
  platform_ = std::make_unique<GlfwPlatform>();
  map_ = std::make_unique<Map>();
  openglScene_ = std::make_shared<OpenglScene>();

  camera_ = std::make_unique<Camera>(glm::vec3(8.0f, 8.0f, 270.0f));

  platform_->Init();

  openglScene_->InitMap();
}

Game::~Game()
{
  platform_->Deinit();
}


int Game::Run()
{
  std::thread renderThread(&Game::RunRenderCycle, this);

  lastCenterChunkCoords_ = CalculateChunkCenter();
  AddChunks(lastCenterChunkCoords_);

  RunSimulationCycle();

  renderThread.join();

  return 0;
}


glm::ivec2 Game::CalculateChunkCenter()
{
  glm::vec3 position = camera_->GetPosition();
  return glm::ivec2((int)position.x / Chunk::Length, (int)position.y / Chunk::Width);
}

void Game::RunSimulationCycle()
{
  while (true)
  {
    glm::ivec2 centerChunk = CalculateChunkCenter();

    if (centerChunk != lastCenterChunkCoords_)
    {
      RemoveChunks(centerChunk, lastCenterChunkCoords_);
      AddChunks(centerChunk);

      lastCenterChunkCoords_ = centerChunk;
    }

    if (window_ && window_->IsWindowShouldClose())
    {
      break;
    }
  }
}

void Game::AddChunks(glm::ivec2 centerChunkCoords)
{
  std::shared_ptr<OpenglMap> map = openglScene_->GetMap();

  for (int x = centerChunkCoords.x - renderRadius_; x <= centerChunkCoords.x + renderRadius_; x++)
  {
    for (int y = centerChunkCoords.y - renderRadius_; y <= centerChunkCoords.y + renderRadius_; y++)
    {
      std::pair<int, int> coordinates = std::make_pair(x, y);

      if (!map->ContainsChunk(coordinates))
      {
        std::shared_ptr<Chunk> chunk = map_->GetChunk(coordinates);
        map->EnqueueChunkAdd(chunk, coordinates);
      }
    }
  }
}

void Game::RemoveChunks(glm::ivec2 CenterChunkCoords, glm::ivec2 lastCenterChunkCoords)
{
  std::shared_ptr<OpenglMap> map = openglScene_->GetMap();
  glm::ivec2 xBorders = glm::ivec2(CenterChunkCoords.x - renderRadius_, CenterChunkCoords.x + renderRadius_);
  glm::ivec2 yBorders = glm::ivec2(CenterChunkCoords.y - renderRadius_, CenterChunkCoords.y + renderRadius_);

  for (int x = lastCenterChunkCoords.x - renderRadius_; x <= lastCenterChunkCoords.x + renderRadius_; x++)
  {
    for (int y = lastCenterChunkCoords.y - renderRadius_; y <= lastCenterChunkCoords.y + renderRadius_; y++)
    {
      if (x < xBorders.x || x > xBorders.y ||
          y < yBorders.x || y > yBorders.y)
      {
        std::pair<int, int> coordinates = std::make_pair(x, y);
        map->EnqueueChunkRemove(coordinates);
      }
    }
  }
}

void Game::RunRenderCycle()
{
  window_ = platform_->CreateWindow(framebufferWidth_, framebufferHeight_, "Title");
  window_->MakeCurrentContext();

  OpenglRenderSystem renderSystem;
  renderSystem.Init();

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;

  ImGui::StyleColorsDark();

  window_->InitImgui();
  // GL 3.0 + GLSL 130
  const char* glsl_version = "#version 130";
  ImGui_ImplOpenGL3_Init(glsl_version);

  bool wireframeMode = false;

  // Set callbacks
  window_->SetFramebufferCallback(
    [this](int width, int height)
    {
      framebufferWidth_ = width;
      framebufferHeight_ = height;

      glViewport(0, 0, width, height);
    }
  );
  window_->SetKeyCallback(
    [this, &renderSystem, &wireframeMode](int keycode, int scancode, int action, int mods)
    {
      if (action != GLFW_PRESS || window_ == nullptr)
        return;

      if (keycode == GLFW_KEY_ESCAPE)
      {
        window_->SetWindowShouldClose(true);
      }

      if (keycode == GLFW_KEY_X)
      {
        wireframeMode = !wireframeMode;
        renderSystem.SetWireframeMode(wireframeMode);
      }

      if (keycode == GLFW_KEY_L)
      {
        SwitchCursorMode();
      }
    }
  );
  window_->SetCursorPositionCallback(
    [this](double xpos, double ypos)
    {
      float xposF = static_cast<float>(xpos);
      float yposF = static_cast<float>(ypos);

      if (firstMouse_)
      {
        lastX_ = xposF;
        lastY_ = yposF;
        firstMouse_ = false;
      }

      float xoffset = xposF - lastX_;
      float yoffset = lastY_ - yposF; // reversed since y-coordinates go from bottom to top

      lastX_ = xposF;
      lastY_ = yposF;

      if (!isCursorEnabled_)
      {
        camera_->ProcessMouseMovement(xoffset, yoffset);
      }
    }
  );
  window_->SetScrollCallback(
    [this](double xoffset, double yoffset)
    {
      camera_->ProcessMouseScroll(static_cast<float>(yoffset));
    }
  );

  std::vector<const char*> paths{ PPCAT(BLOCK_TEXTURES_DIR, BRICK_TEXTURE), PPCAT(BLOCK_TEXTURES_DIR, DIRT_TEXTURE), PPCAT(BLOCK_TEXTURES_DIR, URAN_TEXTURE) };
  OpenglTextureArray texAr(paths, 64, 64);

  bool showImguiWindow = true;

  while (!window_->IsWindowShouldClose())
  {
    renderSystem.StartFrame();

    glfwPollEvents();

    float currentFrame = static_cast<float>(platform_->GetTime());
    deltaTime_ = currentFrame - lastFrame_;
    lastFrame_ = currentFrame;

    ProcessInput();

    renderSystem.Clear();

    float framebufferRatio = (float)framebufferWidth_ / (float)framebufferHeight_;

    openglScene_->GetMap()->ProcessQueues();

    renderSystem.RenderMap(openglScene_->GetMap(), camera_.get(), framebufferRatio);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(0, 0));
    ImGui::Begin("Info");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::Text("Rendered triangles: %d", renderSystem.GetFrameTrianlgesNumber());
    ImGui::Text("Camera position: %.2f %.2f %.2f", camera_->GetPosition().x, camera_->GetPosition().y, camera_->GetPosition().z);
    ImGui::Text("Camera direction: %.2f %.2f %.2f", camera_->GetForward().x, camera_->GetForward().y, camera_->GetForward().z);
    ImGui::End();

    // Render imgui ui
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    renderSystem.FinishFrame();

    window_->SwapBuffers();
  }

  renderSystem.Deinit();
}

void Game::ProcessInput()
{
  if (window_->GetKeyState(GLFW_KEY_W) == GLFW_PRESS)
  {
    camera_->ProcessKeyboard(Camera::FORWARD, deltaTime_);
  }
  if (window_->GetKeyState(GLFW_KEY_S) == GLFW_PRESS)
  {
    camera_->ProcessKeyboard(Camera::BACKWARD, deltaTime_);
  }
  if (window_->GetKeyState(GLFW_KEY_A) == GLFW_PRESS)
  {
    camera_->ProcessKeyboard(Camera::LEFT, deltaTime_);
  }
  if (window_->GetKeyState(GLFW_KEY_D) == GLFW_PRESS)
  {
    camera_->ProcessKeyboard(Camera::RIGHT, deltaTime_);
  }
}

void Game::SwitchCursorMode()
{
  isCursorEnabled_ = !isCursorEnabled_;

  if (isCursorEnabled_)
  {
    window_->SetCursorMode(CursorMode::Normal);
  }
  else
  {
    window_->SetCursorMode(CursorMode::Disabled);
  }
}
