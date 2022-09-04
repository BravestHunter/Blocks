#include "imgui_list_box.hpp"


namespace blocks
{
  ImguiListBox::ImguiListBox(std::string label, std::vector<std::string> items) : label_(label), itemsNumber_(items.size())
  {
    items_ = new char*[itemsNumber_];

    for (int i = 0; i < itemsNumber_; i++)
    {
      items_[i] = new char[items[i].size() + 1];
      strcpy(items_[i], items[i].c_str());
    }
  }

  ImguiListBox::~ImguiListBox()
  {
    for (int i = 0; i < itemsNumber_; i++)
    {
      delete items_[i];
    }

    delete[] items_;
  }


  void ImguiListBox::Render(const OpenglContext* context)
  {
    if (isVisible_ == false)
    {
      return;
    }

    ImGui::ListBox(label_.c_str(), &selectedItem_, items_, itemsNumber_, 4);
  }


  std::string ImguiListBox::GetSelectedItem() const
  {
    if (itemsNumber_ == 0)
    {
      return "";
    }

    return items_[selectedItem_];
  }
}
