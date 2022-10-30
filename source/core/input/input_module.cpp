#include "input_module.hpp"

#include "environment.hpp"


namespace blocks
{
  InputModule::InputModule()
  {
  }


  void InputModule::SetupWindow(GlfwWindow& window)
  {
    window.SetKeyCallback(
      [this](int keycode, int scancode, int action, int mods)
      {
        if (action == GLFW_RELEASE)
        {
          state_.pressedKeys_.erase(keycode);
        }
        else if (action == GLFW_PRESS)
        {
          state_.pressedKeys_.insert(keycode);
          state_.justPressedKeys_.insert(keycode);
        }
      }
    );

    window.SetMouseButtonCallback(
      [this](int button, int action, int mods)
      {
        if (action == GLFW_RELEASE)
        {
          state_.pressedMouseButtons_.erase(button);
        }
        else if (action == GLFW_PRESS)
        {
          state_.pressedMouseButtons_.insert(button);
          state_.justPressedMouseButtons_.insert(button);
        }
      }
    );

    window.SetCursorPositionCallback(
      [this](double xpos, double ypos)
      {
        const glm::vec2 cursorPosition = glm::vec2(xpos, ypos);
        
        // '+=' instead of '=' because event can be fired multiple times during one Update
        state_.cursorOffset_ += cursorPosition - state_.cursorPosition_;
        state_.cursorPosition_ = cursorPosition;
      }
    );

    window.SetScrollCallback(
      [this](double xoffset, double yoffset)
      {
        const glm::vec2 scrollOffset = glm::vec2(xoffset, yoffset);

        // '+=' instead of '=' because event can be fired multiple times during one Update
        state_.scrollOffset_ += scrollOffset;
      }
    );
  }


  void InputModule::Update()
  {
    state_.justPressedKeys_.clear();
    state_.justPressedMouseButtons_.clear();
    state_.cursorOffset_ = glm::vec2();
    state_.scrollOffset_ = glm::vec2();

    Environment::GetPlatform().ProcessEvents();
  }
}
