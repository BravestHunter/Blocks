#include "glfw_platform.hpp"

#include <exception>
#include <iostream>
#include <format>


GlfwPlatform::GlfwPlatform()
{
  if (instance_ != nullptr)
  {
    throw std::exception("Only one instance of GLFW platform system allowed");
  }
  instance_ = this;

  int major, minor, revision;
  glfwGetVersion(&major, &minor, &revision);
  std::cout << std::format("Running GLFW {0}.{1}.{2}\n", major, minor, revision) << std::endl;
}

GlfwPlatform::~GlfwPlatform()
{
  if (isInitialized_)
  { 
    Deinit();
  }

  instance_ = nullptr;
}


void GlfwPlatform::Init()
{
  if (isInitialized_)
  {
    throw std::exception("GLFW platform system is already initialized");
  }

  if (!glfwInit())
  {
    throw std::exception("Failed to initialize GLFW platform system");
  }

  // Add init hints here

  isInitialized_ = true;

  glfwSetErrorCallback(ErrorCallback);
}

void GlfwPlatform::Deinit()
{
  if (!isInitialized_)
  {
    throw std::exception("GLFW platform system was not initialized");
  }

  glfwTerminate();

  isInitialized_ = false;
}

bool GlfwPlatform::IsInitialized()
{
  return isInitialized_;
}


GlfwWindow GlfwPlatform::CreateWindow(int width, int height, std::string title)
{
  glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);

  GLFWwindow* windowPtr = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
  if (!windowPtr)
  {
    throw std::exception("Failed to create GLFW window");
  }

  glfwSetInputMode(windowPtr, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  return GlfwWindow(windowPtr);
}


int GlfwPlatform::GetError()
{
  const char* description;
  int code = glfwGetError(&description);

  if (isWritingErrors_)
  {
    std::cout << "Glfw error; code " << code << "; " << description;
  }

  return code;
}

double GlfwPlatform::GetTime()
{
  return glfwGetTime();
}


GlfwPlatform* GlfwPlatform::instance_ = nullptr;


void GlfwPlatform::ErrorCallback(int code, const char* description)
{
  if (GlfwPlatform::instance_->isWritingErrors_)
  {
    std::cout << "GLFW error; code " << code << "; " << description;
  }
}
