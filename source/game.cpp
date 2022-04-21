#include "game.hpp"

#include <string>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "resourceConfig.h"


Game::Game(int width, int height) : framebufferWidth_(width), framebufferHeight_(height), lastX_(width / 2), lastY_(height / 2)
{
  platform_ = std::make_unique<GlfwPlatform>();
  camera_ = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 3.0f));
}

Game::~Game()
{
  
}


int Game::Run()
{
  platform_->Init();

  std::shared_ptr<GlfwWindow> window = platform_->CreateWindow(framebufferWidth_, framebufferHeight_, "Title");
  window->MakeCurrentContext();

  OpenglRenderSystem renderSystem;
  renderSystem.Init();

  // Set callbacks
  window->SetFramebufferCallback(
    [this](int width, int height)
    {
      framebufferWidth_ = width;
      framebufferHeight_ = height;

      glViewport(0, 0, width, height);
    }
  );
  window->SetKeyCallback(
    [window](int keycode, int scancode, int action, int mods) 
    {
      if (action != GLFW_PRESS || window == nullptr)
        return;
  
      if (keycode == GLFW_KEY_ESCAPE)
      {
        window->SetWindowShouldClose(true);
      }
    }
  );
  window->SetCursorPositionCallback(
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

      camera_->ProcessMouseMovement(xoffset, yoffset);
    }
  );
  window->SetScrollCallback(
    [this](double xoffset, double yoffset)
    {
      camera_->ProcessMouseScroll(static_cast<float>(yoffset));
    }
  );


  while (!window->IsWindowShouldClose())
  {
    glfwPollEvents();

    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime_ = currentFrame - lastFrame_;
    lastFrame_ = currentFrame;

    processInput(window);

    renderSystem.RenderScene(camera_.get(), (float)framebufferWidth_/(float)framebufferHeight_);

    window->SwapBuffers();
  }

  renderSystem.Deinit();
  platform_->Deinit();

  return 0;
}

void Game::processInput(std::shared_ptr<GlfwWindow> window)
{
  if (window->GetKeyState(GLFW_KEY_W) == GLFW_PRESS)
  {
    camera_->ProcessKeyboard(Camera::FORWARD, deltaTime_);
  }
  if (window->GetKeyState(GLFW_KEY_S) == GLFW_PRESS)
  {
    camera_->ProcessKeyboard(Camera::BACKWARD, deltaTime_);
  }
  if (window->GetKeyState(GLFW_KEY_A) == GLFW_PRESS)
  {
    camera_->ProcessKeyboard(Camera::LEFT, deltaTime_);
  }
  if (window->GetKeyState(GLFW_KEY_D) == GLFW_PRESS)
  {
    camera_->ProcessKeyboard(Camera::RIGHT, deltaTime_);
  }
}
