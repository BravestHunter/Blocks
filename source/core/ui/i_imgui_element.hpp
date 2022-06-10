#pragma once

#include "imgui.h"


namespace blocks
{
  class IImguiElement
  {
  public:
    virtual ~IImguiElement() {}

    virtual void Render() = 0;
  };
}
