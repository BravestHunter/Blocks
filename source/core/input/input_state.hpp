#pragma once

#include <set>

#include "glm/glm.hpp"


namespace blocks
{
  class InputModule;

  class InputState
  {
    friend InputModule;

  public:
    inline bool IsKeyPressed(int keyCode) const;
    inline bool IsKeyJustPressed(int keyCode) const;
    inline bool IsMouseButtonPressed(int buttonCode) const;
    inline bool IsMouseButtonJustPressed(int buttonCode) const;
    inline glm::vec2 GetCursorPosition() const;
    inline glm::vec2 GetCursorOffset() const;
    inline glm::vec2 GetScrollOffset() const;

  private:
    std::set<int> pressedKeys_;
    std::set<int> justPressedKeys_;
    std::set<int> pressedMouseButtons_;
    std::set<int> justPressedMouseButtons_;
    glm::vec2 cursorPosition_;
    glm::vec2 cursorOffset_;
    glm::vec2 scrollOffset_;
  };


  bool InputState::IsKeyPressed(int keyCode) const
  {
    return pressedKeys_.contains(keyCode);
  }

  bool InputState::IsKeyJustPressed(int keyCode) const
  {
    return justPressedKeys_.contains(keyCode);
  }

  bool InputState::IsMouseButtonPressed(int buttonCode) const
  {
    return pressedMouseButtons_.contains(buttonCode);
  }

  bool InputState::IsMouseButtonJustPressed(int buttonCode) const
  {
    return justPressedMouseButtons_.contains(buttonCode);
  }

  glm::vec2 InputState::GetCursorPosition() const
  {
    return cursorPosition_;
  }

  glm::vec2 InputState::GetCursorOffset() const
  {
    return cursorOffset_;
  }

  glm::vec2 InputState::GetScrollOffset() const
  {
    return scrollOffset_;
  }
}
