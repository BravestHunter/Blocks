#pragma once

#include <string>

#include "i_imgui_element.hpp"


namespace blocks
{
  class ImguiInputText : public IImguiElement
  {
  public:
    ImguiInputText(int maxLength, std::string label);
    ~ImguiInputText();

    virtual void Render() override;

    std::string GetText();

  private:
    char* buffer_;
    int bufferLength_;
    std::string text_;
    std::string label_;
  };
}
