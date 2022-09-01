#pragma once

#include <string>
#include <vector>

#include "i_imgui_element.hpp"


namespace blocks
{
  class ImguiListBox : public IImguiElement
  {
  public:
    ImguiListBox(std::string label, std::vector<std::string> items);
    ~ImguiListBox();

    virtual void Render() override;

    std::string GetSelectedItem() const;

  private:
    std::string label_;
    char** items_;
    int itemsNumber_;
    int selectedItem_ = 0;
  };
}
