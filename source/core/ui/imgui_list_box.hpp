#pragma once

#include <string>
#include <vector>

#include "base_imgui_element.hpp"


namespace blocks
{
  class ImguiListBox : public BaseImguiElement
  {
  public:
    ImguiListBox(std::string label, std::vector<std::string> items);
    virtual ~ImguiListBox() override;

    virtual void Render() override;

    std::string GetSelectedItem() const;

  private:
    std::string label_;
    char** items_;
    int itemsNumber_;
    int selectedItem_ = 0;
  };
}
