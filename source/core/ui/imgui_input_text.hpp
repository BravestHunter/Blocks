#pragma once

#include <string>

#include "base_imgui_element.hpp"


namespace blocks
{
  class ImguiInputText : public BaseImguiElement
  {
  public:
    ImguiInputText(int maxLength, std::string label);
    virtual ~ImguiInputText() override;

    virtual void Render(const OpenglContext* context) override;

    std::string GetText() const;

  private:
    char* buffer_;
    int bufferLength_;
    std::string text_;
    std::string label_;
  };
}
