#pragma once

#include <memory>
#include <string>

#include "glfw_headers.hpp"
#include "glfw_window.hpp"


namespace blocks
{
  class GlfwPlatform
  {
  public:
    GlfwPlatform();
    GlfwPlatform(const GlfwPlatform&) = delete;
    GlfwPlatform(GlfwPlatform&& other) = delete;
    GlfwPlatform& operator=(const GlfwPlatform&) = delete;
    GlfwPlatform& operator=(GlfwPlatform&& other) = delete;
    ~GlfwPlatform();

    void Init();
    void Deinit();
    bool IsInitialized();

    void ProcessEvents();

    GlfwWindow CreateWindow(int width, int height, std::string title);

    int GetError();
    double GetTime();

  private:
    static void ErrorCallback(int code, const char* description);

    bool isInitialized_ = false;
    bool isWritingErrors_ = true;

    static GlfwPlatform* instance_;
  };
}
