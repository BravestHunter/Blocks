#pragma once

#include <string>
#include <functional>

#include "i_imgui_element.hpp"


namespace blocks
{
  class ImguiText : public IImguiElement
  {
  public:
    ImguiText(std::string text);
    ImguiText(std::function<std::string()> updateStringCallback);

    virtual void Render() override;

  private:
    std::string text_;
    std::function<std::string()> updateStringCallback_;
  };
}
