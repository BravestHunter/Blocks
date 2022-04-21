#pragma once

#include <memory>
#include <string>

#include "glfw_headers.hpp"
#include "glfw_window.hpp"


class GlfwPlatform 
{
public:
  GlfwPlatform();
  ~GlfwPlatform();

  void Init();
  void Deinit();
  
  std::unique_ptr<GlfwWindow> CreateWindow(int width, int height, std::string title);

  int GetError();

  bool IsInitialized();
  bool IsWriteErrors();

private:
  bool isInitialized_ = false;
  bool writeErrors_ = true;
};