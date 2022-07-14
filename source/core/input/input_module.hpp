#pragma once

#include "platform/glfw_window.hpp"

#include "input_state.hpp"


namespace blocks
{
  class InputModule
  {
  public:
    InputModule();

    void SetupWindow(GlfwWindow& window);

    void Update();
    InputState& GetState();

  private:
    InputState state_;
  };
}
