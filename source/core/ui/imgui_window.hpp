#pragma once

#include <string>
#include <vector>
#include <memory>

#include <glm/glm.hpp>

#include "base_imgui_element.hpp"


namespace blocks
{
  class ImguiWindow : BaseImguiElement
  {
  public:
    ImguiWindow(glm::vec2 anchor = glm::vec2(0.0f, 0.0f), glm::vec2 pivot = glm::vec2(0.0f, 0.0f));
    ImguiWindow(std::string title, glm::vec2 anchor = glm::vec2(0.0f, 0.0f), glm::vec2 pivot = glm::vec2(0.0f, 0.0f));
    virtual ~ImguiWindow() override {}

    virtual void Render(const OpenglContext* context) override;

    void AddElement(std::shared_ptr<BaseImguiElement> element);

  private:
    bool hasTitle_ = true;
    std::string title_;
    glm::vec2 anchor_;
    glm::vec2 pivot_;
    std::vector<std::shared_ptr<BaseImguiElement>> elements_;
  };
}
