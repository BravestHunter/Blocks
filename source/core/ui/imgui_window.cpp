#include "imgui_window.hpp"


namespace blocks
{
  ImguiWindow::ImguiWindow(glm::vec2 anchor, glm::vec2 pivot) :
    hasTitle_(false), 
    title_(" "), // Name shoudn't be empty
    anchor_(anchor), 
    pivot_(pivot)
  {

  }

  ImguiWindow::ImguiWindow(std::string title, glm::vec2 anchor, glm::vec2 pivot) :
    hasTitle_(true), 
    title_(title), 
    anchor_(anchor), 
    pivot_(pivot)
  {

  }


  void ImguiWindow::Render(const OpenglContext* context)
  {
    if (isVisible_ == false)
    {
      return;
    }

    ImVec2 position(context->viewportSize.x * anchor_.x, context->viewportSize.y * anchor_.y);
    ImVec2 pivot(pivot_.x, pivot_.y);
    ImGui::SetNextWindowPos(position, 0, pivot);
    ImVec2 size(0.0f, 0.0f);
    ImGui::SetNextWindowSize(size);

    ImGuiWindowFlags flags = 0;
    if (hasTitle_ == false)
    {
      flags |= ImGuiWindowFlags_NoTitleBar;
    }

    ImGui::Begin(title_.c_str(), nullptr, flags);
    if (elements_.size() > 0)
    {
      for (std::shared_ptr<BaseImguiElement> element : elements_)
      {
        element->Render(context);
      }
    }
    ImGui::End();
  }


  void ImguiWindow::AddElement(std::shared_ptr<BaseImguiElement> element)
  {
    elements_.push_back(element);
  }
}
