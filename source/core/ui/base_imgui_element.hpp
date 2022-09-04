#pragma once

#include "imgui.h"

#include "render/opengl_context.hpp"


namespace blocks
{
  class BaseImguiElement
  {
  public:
    BaseImguiElement(bool isVisible = true) : isVisible_(isVisible) {}
    virtual ~BaseImguiElement() {}

    virtual void Render(const OpenglContext* context) = 0;

    inline bool IsVisible();

    inline void SetVisibility(bool isVisible);

  protected:
    bool isVisible_;
  };


  bool BaseImguiElement::IsVisible()
  {
    return isVisible_;
  }


  void BaseImguiElement::SetVisibility(bool isVisible)
  {
    isVisible_ = isVisible;
  }
}
