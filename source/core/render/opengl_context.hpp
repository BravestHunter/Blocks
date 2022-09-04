#pragma once

#include <thread>

#include <glm/glm.hpp>

#include "platform/glfw_window.hpp"


namespace blocks
{
  struct OpenglContext
  {
    GlfwWindow& window_;
    std::thread::id threadId_;
    glm::ivec2 viewportSize;
  };
}
