#pragma once

#include <thread>

#include "platform/glfw_window.hpp"


namespace blocks
{
  struct OpenglContext
  {
    GlfwWindow& window_;
    std::thread::id threadId_;
  };
}
