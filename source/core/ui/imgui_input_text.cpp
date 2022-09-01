#include "imgui_input_text.hpp"


namespace blocks
{
  ImguiInputText::ImguiInputText(int maxLength, std::string label) : bufferLength_(maxLength + 1), label_(label)
  {
    buffer_ = new char[bufferLength_];
    buffer_[0] = 0;
  }

  ImguiInputText::~ImguiInputText()
  {
    delete buffer_;
  }


  void ImguiInputText::Render()
  {
    ImGui::InputText(label_.c_str(), buffer_, bufferLength_);
  }


  std::string ImguiInputText::GetText() const
  {
    return std::string(buffer_);
  }
}
