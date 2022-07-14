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
    bool IsKeyPressed(int keyCode) const;
    bool IsKeyJustPressed(int keyCode) const;
    bool IsMouseButtonPressed(int buttonCode) const;
    bool IsMouseButtonJustPressed(int buttonCode) const;
    glm::vec2 GetCursorPosition() const;
    glm::vec2 GetCursorOffset() const;
    glm::vec2 GetScrollOffset() const;

  private:
    std::set<int> pressedKeys_;
    std::set<int> justPressedKeys_;
    std::set<int> pressedMouseButtons_;
    std::set<int> justPressedMouseButtons_;
    glm::vec2 cursorPosition_;
    glm::vec2 cursorOffset_;
    glm::vec2 scrollOffset_;
  };
}
