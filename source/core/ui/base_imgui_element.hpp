#pragma once

#include "imgui.h"


namespace blocks
{
  class BaseImguiElement
  {
  public:
    BaseImguiElement(bool isVisible = true) : isVisible_(isVisible) {}
    virtual ~BaseImguiElement() {}

    virtual void Render() = 0;

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
