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
    inline const InputState& GetState() const;

  private:
    InputState state_;
  };


  const InputState& InputModule::GetState() const
  {
    return state_;
  }
}
