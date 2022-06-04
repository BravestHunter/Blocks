#include "imgui_window.hpp"


namespace blocks
{
  ImguiWindow::ImguiWindow(std::string title) : title_(title)
  {

  }


  void ImguiWindow::Render()
  {
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(0, 0));

    ImGui::Begin(title_.c_str());
    if (elements_.size() > 0)
    {
      for (std::shared_ptr<IImguiElement> element : elements_)
      {
        element->Render();
      }
    }
    ImGui::End();
  }


  void ImguiWindow::AddElement(std::shared_ptr<IImguiElement> element)
  {
    elements_.push_back(element);
  }
}
