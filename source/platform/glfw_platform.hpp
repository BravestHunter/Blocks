#pragma once

#include <memory>
#include <string>

#include "enviroment_system_interface.hpp"
#include "glfw_headers.hpp"
#include "glfw_window.hpp"


class GlfwPlatform : public EnviromentSystemInterface
{
public:
  GlfwPlatform();
  GlfwPlatform(const GlfwPlatform&) = delete;
  GlfwPlatform(GlfwPlatform&& other) = delete;
  GlfwPlatform& operator=(const GlfwPlatform&) = delete;
  GlfwPlatform& operator=(GlfwPlatform&& other) = delete;
  ~GlfwPlatform() override;

  void Init() override;
  void Deinit() override;
  bool IsInitialized() override;

  GlfwWindow CreateWindow(int width, int height, std::string title);

  int GetError();
  double GetTime();

private:
  static void ErrorCallback(int code, const char* description);

  bool isInitialized_ = false;
  bool isWritingErrors_ = true;

  static GlfwPlatform* instance_;
};
