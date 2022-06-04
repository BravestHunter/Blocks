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
  camera_(std::make_shared<Camera>(glm::vec3(8.0f, 8.0f, 270.0f))),
  framebufferWidth_(width), 
  framebufferHeight_(height), 
  lastMouseX_(width / 2), 
  lastMouseY_(height / 2)
{
  context_.scene = CreateMainMenuScene();
  context_.camera = camera_;
  context_.playerBounds = AABB(glm::vec3(-0.25f, -0.25f, -0.25f), glm::vec3(0.25f, 0.25f, 0.25f));
  context_.lastMouseX = width / 2;
  context_.lastMouseY = height / 2;

  resourceBase_.SetUp(RESOURCE_BASE_PATH);
}

Game::~Game()
{
  // Nothing to do here
}

int Game::Run()
{

  std::thread renderThread(&Game::RunRenderCycle, this);
  std::thread fixedUpdateThread(&Game::RunFixedUpdateCycle, this);

  RunSimulationCycle();

  renderThread.join();
  fixedUpdateThread.join();

  return 0;
}


glm::ivec2 Game::CalculateChunkCenter()
{
  glm::vec3 position = camera_->GetPosition();
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

    if (context_.scene->ContainsMap())
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
  std::shared_ptr<Map> map = context_.scene->GetMap();
  std::shared_ptr<OpenglMap> openglMap = renderModule_.GetOpenglScene()->GetMap();

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
  std::shared_ptr<OpenglMap> openglMap = renderModule_.GetOpenglScene()->GetMap();
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
  GlfwPlatform& platform = Enviroment::GetPlatformSystem();

  GlfwWindow window = platform.CreateWindow(framebufferWidth_, framebufferHeight_, "Blocks Game");
  window.SetCursorMode(CursorMode::Normal);
  window.SetCurrentContext();

  renderModule_.SetContext(window);
  renderModule_.InitResources();

  playerControlModule_.SetCallbacks(window, context_, renderModule_);

  // Set callbacks
  window.SetKeyCallback(
    [this, &window](int keycode, int scancode, int action, int mods)
    {
      if (action != GLFW_PRESS)
        return;

      if (keycode == GLFW_KEY_ESCAPE)
      {
        window.SetWindowShouldClose(true);
      }

      if (keycode == GLFW_KEY_L)
      {
        SwitchCursorMode(window);
      }
    }
  );

  std::shared_ptr<BlockSet> blockSet = resourceBase_.LoadBlockSet(resourceBase_.GetBlockSetNames()->front());
  renderModule_.GetOpenglScene()->GetMap()->SetBlockSet(blockSet);

  float lastTime = (float)platform.GetTime();
  while (isRunning_)
  {
    float currentTime = (float)platform.GetTime();
    float delta = currentTime - lastTime;
    lastTime = currentTime;

    platform.ProcessEvents();
    ProcessInput(window);

    sceneMutex_.lock();
    renderModule_.Update(delta, context_);
    sceneMutex_.unlock();

    window.SwapBuffers();

    if (window.IsWindowShouldClose())
    {
      isRunning_ = false;
    }
  }

  renderModule_.FreeResources();
}

void Game::ProcessInput(GlfwWindow& window)
{
  context_.isWPressed = window.GetKeyState(GLFW_KEY_W) == GLFW_PRESS;
  context_.isSPressed = window.GetKeyState(GLFW_KEY_S) == GLFW_PRESS;
  context_.isAPressed = window.GetKeyState(GLFW_KEY_A) == GLFW_PRESS;
  context_.isDPressed = window.GetKeyState(GLFW_KEY_D) == GLFW_PRESS;
}

void Game::SwitchCursorMode(GlfwWindow& window)
{
  context_.isCursorEnabled = !context_.isCursorEnabled;

  if (context_.isCursorEnabled)
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

    playerControlModule_.Update(0.016f, context_);

    mut.lock();
    cv.wait_until(lk, next, [] {return false; });
    next += delta{ 1 };
  }
}

void Game::RequestScene(std::shared_ptr<Scene> scene)
{
  requestedScene_ = scene;
}

void Game::SetRequestedScene()
{
  sceneMutex_.lock();

  context_.scene = requestedScene_;
  requestedScene_ = nullptr;

  if (context_.scene->ContainsMap())
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

  std::shared_ptr<ImguiText> cameraPositionText = std::make_shared<ImguiText>(
    [this]()
    {
      return std::format("Camera position: {0:.2f} {1:.2f} {2:.2f}", camera_->GetPosition().x, camera_->GetPosition().y, camera_->GetPosition().z);
    }
  );
  window->AddElement(cameraPositionText);

  std::shared_ptr<ImguiText> cameraDirectionText = std::make_shared<ImguiText>(
    [this]()
    {
      return  std::format("Camera direction: {0:.2f} {1:.2f} {2:.2f}", camera_->GetForward().x, camera_->GetForward().y, camera_->GetForward().z);
    }
  );
  window->AddElement(cameraDirectionText);

  std::shared_ptr<ImguiText> seedText = std::make_shared<ImguiText>(
    [this]()
    {
      return  std::format("Map seed: {}", context_.scene->GetMap()->GetSeed());
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

      SaveMap(context_.scene->GetMap());
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
