#pragma once

#include <string>
#include <functional>

#include "base_imgui_element.hpp"


namespace blocks
{
  class ImguiButton : public BaseImguiElement
  {
  public:
    ImguiButton(std::string text, std::function<void()> callback);
    virtual ~ImguiButton() override {}

    virtual void Render() override;

  private:
    std::string text_;
    std::function<void()> callback_;
  };
}
