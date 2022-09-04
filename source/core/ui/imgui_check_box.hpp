#pragma once

#include <string>
#include <functional>

#include "base_imgui_element.hpp"


namespace blocks
{
  class ImguiCheckBox : public BaseImguiElement
  {
  public:
    ImguiCheckBox(bool checked, std::string label, std::function<void(bool)> callback);
    virtual ~ImguiCheckBox() override {}

    virtual void Render(const OpenglContext* context) override;

  private:
    bool checked_;
    std::string label_;
    std::function<void(bool)> callback_;
  };
}
