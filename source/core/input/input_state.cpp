#include "input_state.hpp"


namespace blocks
{
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
