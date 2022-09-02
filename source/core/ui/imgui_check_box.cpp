#include "imgui_check_box.hpp"


namespace blocks
{
  ImguiCheckBox::ImguiCheckBox(bool checked, std::string label, std::function<void(bool)> callback) : checked_(checked), label_(label), callback_(callback)
  {
  }


  void ImguiCheckBox::Render()
  {
    if (ImGui::Checkbox(label_.c_str(), &checked_))
    {
      callback_(checked_);
    }
  }
}
