#include "imgui_text.hpp"


namespace blocks
{
  ImguiText::ImguiText(std::string text) : text_(text)
  {

  }

  ImguiText::ImguiText(std::function<std::string()> updateStringCallback) : updateStringCallback_(updateStringCallback)
  {

  }


  void ImguiText::Render(const OpenglContext* context)
  {
    if (isVisible_ == false)
    {
      return;
    }

    if (updateStringCallback_ != nullptr)
    {
      text_ = updateStringCallback_();
    }

    ImGui::Text(text_.c_str());
  }
}
