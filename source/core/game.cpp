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

    inputModule_.SetupWindow(window_);
  }

  Game::~Game()
  {
    // Nothing to do here
  }

  int Game::Run()
  {
    std::thread presentationThread(&Game::RunPresentationCycle, this);
    std::thread presentationUpdateThread(&Game::RunPresentationUpdateCycle, this);

    RunSimulationCycle();

    presentationThread.join();
    presentationUpdateThread.join();

    return 0;
  }

  void Game::Stop()
  {
    Environment::GetTaskScheduler().Stop();

    isRunning_ = false;
  }


  void Game::RequestScene(std::shared_ptr<Scene> scene)
  {
    requestedScene_ = scene;
  }

  void Game::SetMenuMode(bool isMenuMode)
  {
    if (context_.isMenuMode == isMenuMode)
    {
      return;
    }

    if (!isMenuMode && context_.scene->MenuModeOnlyAllowed())
    {
      return;
    }

    context_.isMenuMode = isMenuMode;

    if (isMenuMode)
    {
      window_.SetCursorMode(CursorMode::Normal);
    }
    else
    {
      window_.SetCursorMode(CursorMode::Disabled);
    }

    context_.scene->OnMenuModeChanged(isMenuMode);
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
        Stop();
        continue;
      }

      mut.unlock();

      {
        if (requestedScene_ != nullptr)
        {
          SetRequestedScene();
        }

        timeModule_.Update();
        const TimeState& timeState = timeModule_.GetState();

        inputModule_.Update();
        const InputState& inputState = inputModule_.GetState();

        if (inputState.IsKeyJustPressed(GLFW_KEY_ESCAPE))
        {
          SetMenuMode(!context_.isMenuMode);
        }

        if (context_.isMenuMode == false)
        {
          simulationModule_.Update(deltaF, timeState, inputState, context_);
        }
      }

      mut.lock();
      cv.wait_until(lk, next, [] {return false; });
      next += delta{ 1 };
    }
  }

  void Game::RunPresentationCycle()
  {
    GlfwPlatform& platform = Environment::GetPlatform();

    window_.SetCursorMode(CursorMode::Normal);
    window_.SetCurrentContext();

    presentationModule_.SetContext(window_);
    presentationModule_.InitResources();

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

  void Game::RunPresentationUpdateCycle()
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
          std::shared_ptr<BaseModelUpdateEvent> e = context_.modelUpdateEventsQueue.Pop();
          presentationModule_.ProcessModelUpdate(e.get(), context_);
        }
      }

      mut.lock();
      cv.wait_until(lk, next, [] {return false; });
      next += delta{ 1 };
    }
  }


  void Game::SetRequestedScene()
  {
    sceneMutex_.lock();

    context_.scene = requestedScene_;
    requestedScene_ = nullptr;

    context_.controlMode = ControlMode::Fly;

    SetMenuMode(context_.scene->MenuModeOnlyAllowed());

    simulationModule_.OnSceneChanged(context_);
    presentationModule_.OnSceneChanged(context_);

    timeModule_.OnSceneChanged();

    sceneMutex_.unlock();
  }
}
