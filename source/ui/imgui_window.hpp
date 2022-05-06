#pragma once

#include <string>
#include <vector>
#include <memory>

#include "i_imgui_element.hpp"


class ImguiWindow
{
public:
  ImguiWindow(std::string title);

  void Render();

  void AddElement(std::shared_ptr<IImguiElement> element);

private:
  std::string title_;
  std::vector<std::shared_ptr<IImguiElement>> elements_;
};
