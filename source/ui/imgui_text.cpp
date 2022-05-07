#include "imgui_text.hpp"


ImguiText::ImguiText(std::string text) : text_(text)
{

}

ImguiText::ImguiText(std::function<std::string()> updateStringCallback) : updateStringCallback_(updateStringCallback)
{

}


void ImguiText::Render()
{
  if (updateStringCallback_ != nullptr)
  {
    text_ = updateStringCallback_();
  }

  ImGui::Text(text_.c_str());
}


