#include "glfw_window.hpp"

#include "backends/imgui_impl_glfw.h"


GlfwWindow::GlfwWindow(GLFWwindow* window) : windowPtr_(window)
{
  glfwSetWindowUserPointer(windowPtr_, this);
  
  glfwSetWindowPosCallback(windowPtr_, PositionCallback);
  glfwSetWindowSizeCallback(windowPtr_, SizeCallback);
  glfwSetWindowCloseCallback(windowPtr_, CloseCallback);
  glfwSetWindowRefreshCallback(windowPtr_, RefreshCallback);
  glfwSetWindowFocusCallback(windowPtr_, FocusCallback);
  glfwSetWindowIconifyCallback(windowPtr_, IconifyCallback);
  glfwSetFramebufferSizeCallback(windowPtr_, FramebufferSizeCallback);

  glfwSetKeyCallback(windowPtr_, KeyCallback);
  glfwSetCharCallback(windowPtr_, CharCallback);
  glfwSetMouseButtonCallback(windowPtr_, MouseButtonCallback);
  glfwSetCursorPosCallback(windowPtr_, CursorPositionCallback);
  glfwSetCursorEnterCallback(windowPtr_, CursorEnterCallback);
  glfwSetScrollCallback(windowPtr_, ScrollCallback);
  glfwSetDropCallback(windowPtr_, DropCallback);
}

GlfwWindow::~GlfwWindow()
{
  // Glfw platform can be already terminated
  // But it kinda works fine
  glfwDestroyWindow(windowPtr_);
}


void GlfwWindow::SwapBuffers()
{
  glfwSwapBuffers(windowPtr_);
}

void GlfwWindow::MakeCurrentContext()
{
  glfwMakeContextCurrent(windowPtr_);

  // Disable vsync
  glfwSwapInterval(0.0f);
}

void GlfwWindow::InitImgui()
{
  ImGui_ImplGlfw_InitForOpenGL(windowPtr_, false);
}


bool GlfwWindow::IsWindowShouldClose()
{
  return glfwWindowShouldClose(windowPtr_);
}

void GlfwWindow::SetWindowShouldClose(bool value)
{
  glfwSetWindowShouldClose(windowPtr_, value);
}


int GlfwWindow::GetKeyState(int keycode)
{
  return glfwGetKey(windowPtr_, keycode);
}


void GlfwWindow::SetPositionCallback(std::function<void(int, int)> func)
{
  positionCallbackFunction_ = func;
}

void GlfwWindow::SetSizeCallback(std::function<void(int, int)> func)
{
  sizeCallbackFunction_ = func;
}

void GlfwWindow::SetCloseCallback(std::function<void()> func)
{
  closeCallbackFunction_ = func;
}

void GlfwWindow::SetRefreshCallback(std::function<void()> func)
{
  refreshCallbackFunction_ = func;
}

void GlfwWindow::SetFocusCallback(std::function<void(int)> func)
{
  focusCallbackFunction_ = func;
}

void GlfwWindow::SetIconifyCallback(std::function<void(int)> func)
{
  iconifyCallbackFunction_ = func;
}

void GlfwWindow::SetFramebufferCallback(std::function<void(int, int)> func)
{
  framebufferCallbackFunction_ = func;
}


void GlfwWindow::SetKeyCallback(std::function<void(int, int, int, int)> func)
{
  keyCallbackFunction_ = func;
}


void GlfwWindow::SetCharCallback(std::function<void(unsigned int)> func)
{
  charCallbackFunction_ = func;
}

void GlfwWindow::SetMouseButtonCallback(std::function<void(int, int, int)> func)
{
  mouseButtonCallbackFunction_ = func;
}

void GlfwWindow::SetCursorPositionCallback(std::function<void(double, double)> func)
{
  cursorPositionCallbackFunction_ = func;
}

void GlfwWindow::SetCursorEnterCallback(std::function<void(int)> func)
{
  cursorEnterCallbackFunction_ = func;
}

void GlfwWindow::SetScrollCallback(std::function<void(double, double)> func)
{
  scrollCallbackFunction_ = func;
}

void GlfwWindow::SetDropCallback(std::function<void(int, const char**)> func)
{
  dropCallbackFunction_ = func;
}


void GlfwWindow::PositionCallback(GLFWwindow* window, int xpos, int ypos)
{
  GlfwWindow* windowInstance = static_cast<GlfwWindow*>(glfwGetWindowUserPointer(window));

  if (windowInstance->positionCallbackFunction_ != nullptr)
  {
    windowInstance->positionCallbackFunction_(xpos, ypos);
  }
}

void GlfwWindow::SizeCallback(GLFWwindow* window, int width, int height)
{
  GlfwWindow* windowInstance = static_cast<GlfwWindow*>(glfwGetWindowUserPointer(window));

  if (windowInstance->sizeCallbackFunction_ != nullptr)
  {
    windowInstance->sizeCallbackFunction_(width, height);
  }
}

void GlfwWindow::CloseCallback(GLFWwindow* window)
{
  GlfwWindow* windowInstance = static_cast<GlfwWindow*>(glfwGetWindowUserPointer(window));

  if (windowInstance->closeCallbackFunction_ != nullptr)
  {
    windowInstance->closeCallbackFunction_();
  }
}

void GlfwWindow::RefreshCallback(GLFWwindow* window)
{
  GlfwWindow* windowInstance = static_cast<GlfwWindow*>(glfwGetWindowUserPointer(window));

  if (windowInstance->refreshCallbackFunction_ != nullptr)
  {
    windowInstance->refreshCallbackFunction_();
  }
}

void GlfwWindow::FocusCallback(GLFWwindow* window, int focused)
{
  GlfwWindow* windowInstance = static_cast<GlfwWindow*>(glfwGetWindowUserPointer(window));

  if (windowInstance->focusCallbackFunction_ != nullptr)
  {
    windowInstance->focusCallbackFunction_(focused);
  }
}

void GlfwWindow::IconifyCallback(GLFWwindow* window, int iconified)
{
  GlfwWindow* windowInstance = static_cast<GlfwWindow*>(glfwGetWindowUserPointer(window));

  if (windowInstance->iconifyCallbackFunction_ != nullptr)
  {
    windowInstance->iconifyCallbackFunction_(iconified);
  }
}

void GlfwWindow::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
  GlfwWindow* windowInstance = static_cast<GlfwWindow*>(glfwGetWindowUserPointer(window));

  if (windowInstance->framebufferCallbackFunction_ != nullptr)
  {
    windowInstance->framebufferCallbackFunction_(width, height);
  }
}


void GlfwWindow::KeyCallback(GLFWwindow* window, int keycode, int scancode, int action, int mods)
{
  GlfwWindow* windowInstance = static_cast<GlfwWindow*>(glfwGetWindowUserPointer(window));

  if (windowInstance->keyCallbackFunction_ != nullptr)
  {
    windowInstance->keyCallbackFunction_(keycode, scancode, action, mods);
  }
}

void GlfwWindow::CharCallback(GLFWwindow* window, unsigned int codepoint)
{
  GlfwWindow* windowInstance = static_cast<GlfwWindow*>(glfwGetWindowUserPointer(window));

  if (windowInstance->charCallbackFunction_ != nullptr)
  {
    windowInstance->charCallbackFunction_(codepoint);
  }
}

void GlfwWindow::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
  GlfwWindow* windowInstance = static_cast<GlfwWindow*>(glfwGetWindowUserPointer(window));

  if (windowInstance->mouseButtonCallbackFunction_ != nullptr)
  {
    windowInstance->mouseButtonCallbackFunction_(button, action, mods);
  }
}

void GlfwWindow::CursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
{
  GlfwWindow* windowInstance = static_cast<GlfwWindow*>(glfwGetWindowUserPointer(window));

  if (windowInstance->cursorPositionCallbackFunction_ != nullptr)
  {
    windowInstance->cursorPositionCallbackFunction_(xpos, ypos);
  }
}

void GlfwWindow::CursorEnterCallback(GLFWwindow* window, int entered)
{
  GlfwWindow* windowInstance = static_cast<GlfwWindow*>(glfwGetWindowUserPointer(window));

  if (windowInstance->cursorEnterCallbackFunction_ != nullptr)
  {
    windowInstance->cursorEnterCallbackFunction_(entered);
  }
}

void GlfwWindow::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
  GlfwWindow* windowInstance = static_cast<GlfwWindow*>(glfwGetWindowUserPointer(window));

  if (windowInstance->scrollCallbackFunction_ != nullptr)
  {
    windowInstance->scrollCallbackFunction_(xoffset, yoffset);
  }
}

void GlfwWindow::DropCallback(GLFWwindow* window, int count, const char** paths)
{
  GlfwWindow* windowInstance = static_cast<GlfwWindow*>(glfwGetWindowUserPointer(window));

  if (windowInstance->dropCallbackFunction_ != nullptr)
  {
    windowInstance->dropCallbackFunction_(count, paths);
  }
}
