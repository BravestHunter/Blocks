#pragma once

#include <thread>

#include "platform/glfw_window.hpp"


struct OpenglContext
{
  GlfwWindow& window_;
  std::thread::id threadId_;
};
