#pragma once

#include <string>
#include <vector>
#include <memory>

#include "base_imgui_element.hpp"


namespace blocks
{
  class ImguiWindow : BaseImguiElement
  {
  public:
    ImguiWindow(std::string title);
    virtual ~ImguiWindow() override {}

    virtual void Render() override;

    void AddElement(std::shared_ptr<BaseImguiElement> element);

  private:
    std::string title_;
    std::vector<std::shared_ptr<BaseImguiElement>> elements_;
  };
}
