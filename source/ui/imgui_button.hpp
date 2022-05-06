#pragma once

#include <string>
#include <functional>

#include "i_imgui_element.hpp"


class ImguiButton : public IImguiElement
{
public:
  ImguiButton(std::string text, std::function<void()> callback);

  virtual void Render() override;

private:
  std::string text_;
  std::function<void()> callback_;
};
