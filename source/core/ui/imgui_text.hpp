#pragma once

#include <string>
#include <functional>

#include "base_imgui_element.hpp"


namespace blocks
{
  class ImguiText : public BaseImguiElement
  {
  public:
    ImguiText(std::string text);
    ImguiText(std::function<std::string()> updateStringCallback);
    virtual ~ImguiText() override {}

    virtual void Render() override;

  private:
    std::string text_;
    std::function<std::string()> updateStringCallback_;
  };
}
