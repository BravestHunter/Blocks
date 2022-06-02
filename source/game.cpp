#include "game.hpp"

#include <memory>
#include <thread>
#include <string>
#include <iostream>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <format>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "imgui_internal.h"

#include "model/chunk.hpp"
#include "render/opengl_chunk.hpp"

#include "compile_utils.hpp"
#include "resourceConfig.h"

#include "io/file_api.hpp"

#include "ui/i_imgui_element.hpp"
#include "ui/imgui_button.hpp"
#include "ui/imgui_text.hpp"
#include "ui/imgui_window.hpp"


Game::Game(int width, int height) : 
  framebufferWidth_(width), 
  framebufferHeight_(height), 
  lastMouseX_(width / 2), 
  lastMouseY_(height / 2)
{
  openglScene_ = std::make_shared<OpenglScene>();
  
  openglScene_->InitMap();

  currentScene_ = CreateMainMenuScene();
}

Game::~Game()
{
  // Nothing to do here
}


void Game::StartSystems()
{
  resourceBase_.SetUp(RESOURCE_BASE_PATH);
}

void Game::StopSystems()
{
}

int Game::Run()
{
  StartSystems();

  std::thread renderThread(&Game::RunRenderCycle, this);
  std::thread fixedUpdateThread(&Game::RunFixedUpdateCycle, this);

  RunSimulationCycle();

  renderThread.join();
  fixedUpdateThread.join();

  StopSystems();

  return 0;
}


glm::ivec2 Game::CalculateChunkCenter()
{
  glm::vec3 position = camera_.GetPosition();
  return glm::ivec2((int)position.x / Chunk::Length, (int)position.y / Chunk::Width);
}

void Game::RunSimulationCycle()
{
  while (isRunning_)
  {
    if (requestedScene_ != nullptr)
    {
      SetRequestedScene();
    }

    if (currentScene_->ContainsMap())
    {
      glm::ivec2 centerChunk = CalculateChunkCenter();
      
      if (centerChunk != lastCenterChunkCoords_)
      {
        RemoveChunks(centerChunk, lastCenterChunkCoords_);
        AddChunks(centerChunk);
      
        lastCenterChunkCoords_ = centerChunk;
      }
    }
  }
}

void Game::AddChunks(glm::ivec2 centerChunkCoords)
{
  std::shared_ptr<Map> map = currentScene_->GetMap();
  std::shared_ptr<OpenglMap> openglMap = openglScene_->GetMap();

  for (int x = centerChunkCoords.x - renderRadius_; x <= centerChunkCoords.x + renderRadius_; x++)
  {
    for (int y = centerChunkCoords.y - renderRadius_; y <= centerChunkCoords.y + renderRadius_; y++)
    {
      std::pair<int, int> coordinates = std::make_pair(x, y);

      if (!openglMap->ContainsChunk(coordinates))
      {
        std::shared_ptr<Chunk> chunk = map->GetChunk(coordinates);
        openglMap->EnqueueChunkAdd(chunk, coordinates);
      }
    }
  }
}

void Game::RemoveChunks(glm::ivec2 CenterChunkCoords, glm::ivec2 lastCenterChunkCoords)
{
  std::shared_ptr<OpenglMap> openglMap = openglScene_->GetMap();
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
        openglMap->EnqueueChunkRemove(coordinates);
      }
    }
  }
}

void Game::RunRenderCycle()
{
  GlfwWindow window = Enviroment::GetPlatformSystem().CreateWindow(framebufferWidth_, framebufferHeight_, "Blocks Game");
  window.SetCursorMode(CursorMode::Normal);
  window.MakeCurrentContext();

  renderSystem_.StartUp();

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;

  ImGui::StyleColorsDark();

  window.InitImgui();
  // GL 3.0 + GLSL 130
  const char* glsl_version = "#version 130";
  ImGui_ImplOpenGL3_Init(glsl_version);

  bool wireframeMode = false;

  // Set callbacks
  window.SetFramebufferCallback(
    [this](int width, int height)
    {
      framebufferWidth_ = width;
      framebufferHeight_ = height;

      glViewport(0, 0, width, height);
    }
  );
  window.SetKeyCallback(
    [this, &window, &wireframeMode](int keycode, int scancode, int action, int mods)
    {
      if (action != GLFW_PRESS)
        return;

      if (keycode == GLFW_KEY_ESCAPE)
      {
        window.SetWindowShouldClose(true);
      }

      if (keycode == GLFW_KEY_X)
      {
        wireframeMode = !wireframeMode;
        renderSystem_.SetWireframeMode(wireframeMode);
      }

      if (keycode == GLFW_KEY_L)
      {
        SwitchCursorMode(window);
      }
    }
  );
  window.SetCursorPositionCallback(
    [this](double xpos, double ypos)
    {
      float xposF = static_cast<float>(xpos);
      float yposF = static_cast<float>(ypos);

      if (firstMouse_)
      {
        lastMouseX_ = xposF;
        lastMouseY_ = yposF;
        firstMouse_ = false;
      }

      float xoffset = xposF - lastMouseX_;
      float yoffset = lastMouseY_ - yposF; // reversed since y-coordinates go from bottom to top

      lastMouseX_ = xposF;
      lastMouseY_ = yposF;

      if (!isCursorEnabled_)
      {
        camera_.ProcessMouseMovement(xoffset, yoffset);
      }
    }
  );
  window.SetScrollCallback(
    [this](double xoffset, double yoffset)
    {
      camera_.ProcessMouseScroll(static_cast<float>(yoffset));
    }
  );
  window.SetMouseButtonCallback(
    [this](int button, int action, int mods)
    {
      if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS) // Place block
      {
        if (currentScene_->ContainsMap() && !isCursorEnabled_)
        {
          BlockLookAt blockLookAt = currentScene_->GetMap()->GetBlockLookAt(Ray(camera_.GetPosition(), camera_.GetForward()));
          if (!blockLookAt.hit)
          {
            return;
          }

          std::pair<int, int> placeChunkPosition = blockLookAt.chunkPosition;
          glm::ivec3 placeBlockPosition = blockLookAt.blockPosition;
          switch (blockLookAt.loockFromDirection)
          {
          case Direction::Forward:
            if (blockLookAt.blockPosition.x == Chunk::Length - 1)
            {
              placeChunkPosition.first += 1;
              placeBlockPosition.x = 0;
            }
            else
            {
              placeBlockPosition.x++;
            }
            break;
          case Direction::Back:
            if (blockLookAt.blockPosition.x == 0)
            {
              placeChunkPosition.first -= 1;
              placeBlockPosition.x = 15;
            }
            else
            {
              placeBlockPosition.x--;
            }
            break;

          case Direction::Right:
            if (blockLookAt.blockPosition.y == Chunk::Width - 1)
            {
              placeChunkPosition.second += 1;
              placeBlockPosition.y = 0;
            }
            else
            {
              placeBlockPosition.y++;
            }
            break;
          case Direction::Left:
            if (blockLookAt.blockPosition.y == 0)
            {
              placeChunkPosition.second -= 1;
              placeBlockPosition.y = 15;
            }
            else
            {
              placeBlockPosition.y--;
            }
            break;

          case Direction::Up:
            if (blockLookAt.blockPosition.z == Chunk::Height - 1)
            {
              return;
            }
            else
            {
              placeBlockPosition.z++;
            }
            break;
          case Direction::Down:
            if (blockLookAt.blockPosition.z == 0)
            {
              return;
            }
            else
            {
              placeBlockPosition.z--;
            }
            break;
          }

          // Check collision
          glm::vec3 worldBlockPosition(placeBlockPosition.x + placeChunkPosition.first * Chunk::Length, placeBlockPosition.y + placeChunkPosition.second * Chunk::Width, placeBlockPosition.z);
          const AABB blockBounds(glm::vec3(worldBlockPosition.x, worldBlockPosition.y, worldBlockPosition.z), glm::vec3(worldBlockPosition.x + 1, worldBlockPosition.y + 1, worldBlockPosition.z + 1));
          const AABB playerBounds(playerBounds_.low + camera_.GetPosition(), playerBounds_.high + camera_.GetPosition());
          if (CheckCollision(blockBounds, playerBounds))
          {
            return;
          }

          std::shared_ptr<Chunk> chunk = currentScene_->GetMap()->GetChunk(placeChunkPosition);
          chunk->blocks[placeBlockPosition.x + placeBlockPosition.y * Chunk::Width + placeBlockPosition.z * Chunk::LayerBlocksNumber] = 1;
          openglScene_->AddChunk(chunk, placeChunkPosition);
        }
      }
      else if (button == GLFW_MOUSE_BUTTON_2 && action == GLFW_PRESS) // Remove block
      {
        if (currentScene_->ContainsMap() && !isCursorEnabled_)
        {
          BlockLookAt blockLookAt = currentScene_->GetMap()->GetBlockLookAt(Ray(camera_.GetPosition(), camera_.GetForward()));
          if (!blockLookAt.hit)
          {
            return;
          }

          std::shared_ptr<Chunk> chunk = currentScene_->GetMap()->GetChunk(blockLookAt.chunkPosition);
          chunk->blocks[blockLookAt.blockPosition.x + blockLookAt.blockPosition.y * Chunk::Width + blockLookAt.blockPosition.z * Chunk::LayerBlocksNumber] = 0;
          openglScene_->AddChunk(chunk, blockLookAt.chunkPosition);
        }
      }
    }
  );

  std::shared_ptr<BlockSet> blockSet = resourceBase_.LoadBlockSet(resourceBase_.GetBlockSetNames()->front());
  openglScene_->GetMap()->SetBlockSet(blockSet);

  while (isRunning_)
  {
    sceneMutex_.lock();

    glfwPollEvents();
    ProcessInput(window);

    renderSystem_.StartFrame();

    renderSystem_.Clear();

    float framebufferRatio = (float)framebufferWidth_ / (float)framebufferHeight_;

    if (openglScene_)
    {
      openglScene_->GetMap()->ProcessQueues();
      renderSystem_.RenderMap(openglScene_->GetMap(), camera_, framebufferRatio);
    }

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    auto imguiWindowsIterator = currentScene_->GetImguiWindowsIterator();
    for (auto it = imguiWindowsIterator.first; it != imguiWindowsIterator.second; it++)
    {
      it->get()->Render();
    }

    // Render imgui ui
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    renderSystem_.FinishFrame();

    window.SwapBuffers();

    sceneMutex_.unlock();

    if (window.IsWindowShouldClose())
    {
      isRunning_ = false;
    }
  }

  renderSystem_.ShutDown();
}

void Game::ProcessInput(GlfwWindow& window)
{
  isWPressed_ = window.GetKeyState(GLFW_KEY_W) == GLFW_PRESS;
  isSPressed_ = window.GetKeyState(GLFW_KEY_S) == GLFW_PRESS;
  isAPressed_ = window.GetKeyState(GLFW_KEY_A) == GLFW_PRESS;
  isDPressed_ = window.GetKeyState(GLFW_KEY_D) == GLFW_PRESS;
}

void Game::SwitchCursorMode(GlfwWindow& window)
{
  isCursorEnabled_ = !isCursorEnabled_;

  if (isCursorEnabled_)
  {
    window.SetCursorMode(CursorMode::Normal);
  }
  else
  {
    window.SetCursorMode(CursorMode::Disabled);
  }
}

void Game::RunFixedUpdateCycle()
{
  std::condition_variable cv;
  std::mutex mut;

  using delta = std::chrono::duration<std::int64_t, std::ratio<1, 60>>;
  auto next = std::chrono::steady_clock::now() + delta{ 1 };
  std::unique_lock<std::mutex> lk(mut);

  while (isRunning_)
  {
    mut.unlock();

    if (currentScene_->ContainsMap())
    {
      MovePlayer();
    }

    mut.lock();
    cv.wait_until(lk, next, [] {return false; });
    next += delta{ 1 };
  }
}

void Game::MovePlayer()
{
  float fixedDelta = 0.016f;
  float velocity = 32.0f * fixedDelta;

  glm::vec3 position = camera_.GetPosition();
  glm::vec3 shift = glm::vec3(0.0f);

  if (isWPressed_)
  {
    shift += camera_.GetForward() * velocity;
  }
  if (isSPressed_)
  {
    shift -= camera_.GetForward() * velocity;
  }
  if (isAPressed_)
  {
    shift -= camera_.GetRight() * velocity;
  }
  if (isDPressed_)
  {
    shift += camera_.GetRight() * velocity;
  }

  position += shift;
  if (!currentScene_->GetMap()->Collides(playerBounds_, position))
  {
    camera_.SetPosition(position);
  }
}


void Game::RequestScene(std::shared_ptr<Scene> scene)
{
  requestedScene_ = scene;
}

void Game::SetRequestedScene()
{
  sceneMutex_.lock();

  currentScene_ = requestedScene_;
  requestedScene_ = nullptr;

  if (currentScene_->ContainsMap())
  {
    lastCenterChunkCoords_ = CalculateChunkCenter();
    AddChunks(lastCenterChunkCoords_);
  }

  sceneMutex_.unlock();
}

std::shared_ptr<Scene> Game::CreateMainMenuScene()
{
  std::shared_ptr<Scene> scene = std::make_shared<Scene>();

  std::shared_ptr<ImguiWindow> window = std::make_shared<ImguiWindow>("Main menu");
  scene->AddImguiWindow(window);

  std::shared_ptr<ImguiButton> createWorldButton = std::make_shared<ImguiButton>(
    "Create new world",
    [this]()
    {
      srand(time(0));
      std::shared_ptr<Scene> worldScene_ = CreateWorldScene(std::make_shared<Map>(rand()));
      RequestScene(worldScene_);
    }
  );
  window->AddElement(createWorldButton);

  std::shared_ptr<ImguiButton> loadWorldButton = std::make_shared<ImguiButton>(
    "Load world",
    [this]()
    {
      if (!isPathExist("map"))
      {
        return;
      }

      std::shared_ptr<Map> map = LoadMap();
      std::shared_ptr<Scene> worldScene_ = CreateWorldScene(map);

      RequestScene(worldScene_);
    }
  );
  window->AddElement(loadWorldButton);

  return scene;
}

std::shared_ptr<Scene> Game::CreateWorldScene(std::shared_ptr<Map> map)
{
  std::shared_ptr<Scene> scene = std::make_shared<Scene>();

  scene->SetMap(map);

  std::shared_ptr<ImguiWindow> window = std::make_shared<ImguiWindow>("Statistics");
  scene->AddImguiWindow(window);

  std::shared_ptr<ImguiText> fpsText = std::make_shared<ImguiText>(
    [this]()
    {
      return std::format("Application average {0:.3f} ms/frame ({1:.0f} FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }
  );
  window->AddElement(fpsText);

  std::shared_ptr<ImguiText> renderStatisticsText = std::make_shared<ImguiText>(
    [this]()
    {
      return  std::format("Rendered triangles: {}", renderSystem_.GetFrameTrianlgesNumber());
    }
  );
  window->AddElement(renderStatisticsText);

  std::shared_ptr<ImguiText> cameraPositionText = std::make_shared<ImguiText>(
    [this]()
    {
      return std::format("Camera position: {0:.2f} {1:.2f} {2:.2f}", camera_.GetPosition().x, camera_.GetPosition().y, camera_.GetPosition().z);
    }
  );
  window->AddElement(cameraPositionText);

  std::shared_ptr<ImguiText> cameraDirectionText = std::make_shared<ImguiText>(
    [this]()
    {
      return  std::format("Camera direction: {0:.2f} {1:.2f} {2:.2f}", camera_.GetForward().x, camera_.GetForward().y, camera_.GetForward().z);
    }
  );
  window->AddElement(cameraDirectionText);

  std::shared_ptr<ImguiText> seedText = std::make_shared<ImguiText>(
    [this]()
    {
      return  std::format("Map seed: {}", currentScene_->GetMap()->GetSeed());
    }
  );
  window->AddElement(seedText);

  std::shared_ptr<ImguiButton> saveButton = std::make_shared<ImguiButton>(
    "Save world",
    [this]()
    {
      if (!isPathExist("map"))
      {
        createDirectory("map");
      }
      else
      {
        for (const std::string& path : getFilesInDirectory("map"))
        {
          removePath(path);
        }
      }

      SaveMap(currentScene_->GetMap());
    }
  );
  window->AddElement(saveButton);

  return scene;
}

std::shared_ptr<Map> Game::LoadMap()
{
  std::string seedStr = readTextFile("map/seed.txt");
  int seed = std::stoi(seedStr);

  std::shared_ptr<Map> map = std::make_shared<Map>(seed);

  for (const std::string& path : getFilesInDirectory("map"))
  {
    if (path == "seed.txt")
    {
      continue;
    }

    std::vector<unsigned char> data = readBinaryFile("map/" + path);
    std::shared_ptr<Chunk> chunk = std::make_shared<Chunk>();
    memcpy(chunk.get(), &data[0], sizeof(Chunk));

    size_t underscorePosition = path.find("_");
    size_t dotPosition = path.find(".");

    std::string xStr = path.substr(0, underscorePosition);
    int x = std::stoi(xStr);
    std::string yStr = path.substr(underscorePosition + 1, dotPosition - underscorePosition - 1);
    int y = std::stoi(yStr);
    std::pair<int, int> position = std::make_pair(x, y);

    map->AddChunk(position, chunk);
  }

  return map;
}

void Game::SaveMap(std::shared_ptr<Map> map)
{
  saveTextFile("map/seed.txt", std::to_string(map->GetSeed()));

  auto chunksIterator = map->GetChunksIterator();
  for (auto it = chunksIterator.first; it != chunksIterator.second; it++)
  {
    std::string path = std::format("map/{0}_{1}.chunk", it->first.first, it->first.second);

    size_t a = sizeof(Chunk);
    std::vector<unsigned char> data(sizeof(Chunk));
    memcpy(&data[0], it->second.get(), sizeof(Chunk));

    saveBinaryFile(path, data);
  }
}
