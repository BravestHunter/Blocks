#include "glfw_platform.hpp"

#include <exception>
#include <iostream>


void ErrorCallback(int code, const char* description);


static std::unique_ptr<GlfwPlatform> platformInstance;


GlfwPlatform::GlfwPlatform()
{
  if (platformInstance.get())
  {
    throw std::exception("Only one instance of glfw platform allowed");
  }

  platformInstance = std::unique_ptr<GlfwPlatform>(this);

  int major, minor, revision;
  glfwGetVersion(&major, &minor, &revision);
  printf("Running against GLFW %i.%i.%i\n", major, minor, revision);
}

GlfwPlatform::~GlfwPlatform()
{
  if (isInitialized_)
  { 
    Deinit();
  }

  platformInstance.release();
}


void GlfwPlatform::Init()
{
  // Add init hints here

  if (glfwInit())
  {
    // Something went wrong
  }

  isInitialized_ = true;

  glfwSetErrorCallback(ErrorCallback);
}

void GlfwPlatform::Deinit()
{
  glfwTerminate();

  isInitialized_ = false;
}


std::unique_ptr<GlfwWindow> GlfwPlatform::CreateWindow(int width, int height, std::string title)
{
  if (!isInitialized_)
  {
    throw std::exception("GLFW must be initialized first");
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* windowPtr = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
  if (!windowPtr)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
  }

  glfwSetInputMode(windowPtr, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  GlfwWindow* window = new GlfwWindow(windowPtr);
  std::unique_ptr<GlfwWindow> result(window);

  return result;
}


int GlfwPlatform::GetError()
{
  if (!isInitialized_)
  {
    throw std::exception("GLFW must be initialized first");
  }

  const char* description;
  int code = glfwGetError(&description);

  if (writeErrors_)
  {
    std::cout << "Glfw error code " << code << ", " << description;
  }

  return code;
}


bool GlfwPlatform::IsInitialized()
{
  return isInitialized_;
}

bool GlfwPlatform::IsWriteErrors()
{
  return writeErrors_;
}


void ErrorCallback(int code, const char* description)
{
  if (platformInstance->IsWriteErrors())
  {
    std::cout << "Glfw error code " << code << "; " << description;
  }
}
