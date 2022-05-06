#pragma once

#include "imgui.h"


class IImguiElement
{
public:
  virtual ~IImguiElement() {}

  virtual void Render() = 0;
};
