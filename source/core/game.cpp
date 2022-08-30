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

#include "io/file_api.hpp"
#include "scene/scene_builder.hpp"


namespace blocks
{
  Game::Game(int width, int height) : 
    window_(Environment::GetPlatform().CreateWindow(width, height, "Blocks Game"))
  {
    context_.scene = SceneBuilder::BuildMainMenuScene(this);
    context_.camera = std::make_shared<Camera>(glm::vec3(8.0f, 8.0f, 270.0f));
    context_.playerBounds = blocks::AABB(glm::vec3(-0.25f, -0.25f, -0.25f), glm::vec3(0.25f, 0.25f, 0.25f));

    inputModule_.SetupWindow(window_);
  }

  Game::~Game()
  {
    // Nothing to do here
  }

  int Game::Run()
  {
    std::thread renderThread(&Game::RunRenderCycle, this);
    std::thread renderUpdateThread(&Game::RunRenderUpdateCycle, this);

    RunSimulationCycle();

    renderThread.join();
    renderUpdateThread.join();

    return 0;
  }

  void Game::RequestScene(std::shared_ptr<Scene> scene)
  {
    requestedScene_ = scene;
  }


  GameContext& Game::GetContext()
  {
    return context_;
  }


  void Game::RunSimulationCycle()
  {
    std::condition_variable cv;
    std::mutex mut;

    using delta = std::chrono::duration<std::int64_t, std::ratio<1, 60>>;
    auto next = std::chrono::steady_clock::now() + delta{ 1 };
    std::unique_lock<std::mutex> lk(mut);

    const static float deltaF = 1.0f / 60.0f;
    long counter = 0;
    while (isRunning_)
    {
      if (window_.IsWindowShouldClose())
      {
        isRunning_ = false;
        continue;
      }

      mut.unlock();

      {
        if (requestedScene_ != nullptr)
        {
          SetRequestedScene();
        }

        inputModule_.Update();
        InputState& inputState = inputModule_.GetState();

        if (inputState.IsKeyJustPressed(GLFW_KEY_ESCAPE))
        {
          window_.SetWindowShouldClose(true);
          isRunning_ = false;
        }

        if (inputState.IsKeyJustPressed(GLFW_KEY_L))
        {
          SwitchCursorMode(window_);
        }

        simulationModule_.Update(deltaF, inputState, context_);
      }

      mut.lock();
      cv.wait_until(lk, next, [] {return false; });
      next += delta{ 1 };
    }
  }

  void Game::RunRenderCycle()
  {
    GlfwPlatform& platform = Environment::GetPlatform();

    window_.SetCursorMode(CursorMode::Normal);
    window_.SetCurrentContext();

    presentationModule_.SetContext(window_);
    presentationModule_.InitResources();
    context_.openglScene = presentationModule_.GetRenderModule().GetOpenglScene();

    float lastTime = (float)platform.GetTime();
    while (isRunning_)
    {
      float currentTime = (float)platform.GetTime();
      float delta = currentTime - lastTime;
      lastTime = currentTime;

      sceneMutex_.lock();
      presentationModule_.Update(delta, context_);
      sceneMutex_.unlock();

      window_.SwapBuffers();
    }

    presentationModule_.FreeResources();
  }

  void Game::RunRenderUpdateCycle()
  {
    std::condition_variable cv;
    std::mutex mut;

    using delta = std::chrono::duration<std::int64_t, std::ratio<1, 16>>;
    auto next = std::chrono::steady_clock::now() + delta{ 1 };
    std::unique_lock<std::mutex> lk(mut);

    const static float deltaF = 1.0f / 16.0f;
    long counter = 0;
    while (isRunning_)
    {
      mut.unlock();

      {
        while (context_.modelUpdateEventsQueue.IsEmpty() == false)
        {
          const ModelUpdateEvent e = context_.modelUpdateEventsQueue.Pop();
          presentationModule_.ProcessModelUpdate(e, context_);
        }

        presentationModule_.GetMapLoadingModule().ProcessChunksToAdd(context_);
      }

      mut.lock();
      cv.wait_until(lk, next, [] {return false; });
      next += delta{ 1 };
    }
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


  void Game::SetRequestedScene()
  {
    sceneMutex_.lock();

    context_.scene = requestedScene_;
    requestedScene_ = nullptr;

    presentationModule_.OnSceneChanged(context_);

    sceneMutex_.unlock();
  }
}
