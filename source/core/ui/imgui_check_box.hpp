#pragma once

#include <string>
#include <functional>

#include "i_imgui_element.hpp"


namespace blocks
{
  class ImguiCheckBox : public IImguiElement
  {
  public:
    ImguiCheckBox(bool checked, std::string label, std::function<void(bool)> callback);

    virtual void Render() override;

  private:
    bool checked_;
    std::string label_;
    std::function<void(bool)> callback_;
  };
}
