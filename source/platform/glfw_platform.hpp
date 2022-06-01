#pragma once

#include <memory>
#include <string>

#include "game_system_interface.hpp"
#include "glfw_headers.hpp"
#include "glfw_window.hpp"


class GlfwPlatform : public GameSystemInterface
{
  friend void ErrorCallback(int code, const char* description);

public:
  GlfwPlatform();
  GlfwPlatform(const GlfwPlatform&) = delete;
  GlfwPlatform(GlfwPlatform&& other) = delete;
  GlfwPlatform& operator=(const GlfwPlatform&) = delete;
  GlfwPlatform& operator=(GlfwPlatform&& other) = delete;
  ~GlfwPlatform() override;

  void StartUp() override;
  void ShutDown() override;
  bool IsWorking() override;

  std::unique_ptr<GlfwWindow> CreateWindow(int width, int height, std::string title);

  int GetError();
  double GetTime();

private:
  bool isRunning_ = false;
  bool isWritingErrors_ = true;
};
