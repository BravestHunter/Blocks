#pragma once

#include <functional>

#include "glm/vec2.hpp"

#include "glfw_headers.hpp"
#include "cursor_mode.hpp"


namespace blocks
{
  class GlfwPlatform;

  class GlfwWindow
  {
    friend GlfwPlatform;

  private:
    GlfwWindow(GLFWwindow* window);

  public:
    GlfwWindow(const GlfwWindow&) = delete;
    GlfwWindow(GlfwWindow&& other);
    GlfwWindow& operator=(const GlfwWindow&) = delete;
    GlfwWindow& operator=(GlfwWindow&& other);
    ~GlfwWindow();

    bool IsReleased();

    void SwapBuffers();
    void SetCurrentContext();
    void InitImgui();

    bool IsWindowShouldClose();
    void SetWindowShouldClose(bool value);

    glm::ivec2 GetSize();
    int GetKeyState(int keycode);
    CursorMode GetCursorMode();

    void SetCursorMode(CursorMode mode);

    void SetPositionCallback(std::function<void(int, int)> func);
    void SetSizeCallback(std::function<void(int, int)> func);
    void SetCloseCallback(std::function<void()> func);
    void SetRefreshCallback(std::function<void()> func);
    void SetFocusCallback(std::function<void(int)> func);
    void SetIconifyCallback(std::function<void(int)> func);
    void SetFramebufferCallback(std::function<void(int, int)> func);

    void SetKeyCallback(std::function<void(int, int, int, int)> func);
    void SetCharCallback(std::function<void(unsigned int)> func);
    void SetMouseButtonCallback(std::function<void(int, int, int)> func);
    void SetCursorPositionCallback(std::function<void(double, double)> func);
    void SetCursorEnterCallback(std::function<void(int)> func);
    void SetScrollCallback(std::function<void(double, double)> func);
    void SetDropCallback(std::function<void(int, const char**)> func);

  private:
    void Release();

    static void PositionCallback(GLFWwindow* window, int xpos, int ypos);
    static void SizeCallback(GLFWwindow* window, int width, int height);
    static void CloseCallback(GLFWwindow* window);
    static void RefreshCallback(GLFWwindow* window);
    static void FocusCallback(GLFWwindow* window, int focused);
    static void IconifyCallback(GLFWwindow* window, int iconified);
    static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

    static void KeyCallback(GLFWwindow* window, int keycode, int scancode, int action, int mods);
    static void CharCallback(GLFWwindow* window, unsigned int codepoint);
    static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void CursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
    static void CursorEnterCallback(GLFWwindow* window, int entered);
    static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    static void DropCallback(GLFWwindow* window, int count, const char** paths);

    GLFWwindow* windowPtr_;
    bool isImguiInitialized_ = false;

    std::function<void(int, int)> positionCallbackFunction_ = nullptr;
    std::function<void(int, int)> sizeCallbackFunction_ = nullptr;
    std::function<void()> closeCallbackFunction_ = nullptr;
    std::function<void()> refreshCallbackFunction_ = nullptr;
    std::function<void(int)> focusCallbackFunction_ = nullptr;
    std::function<void(int)> iconifyCallbackFunction_ = nullptr;
    std::function<void(int, int)> framebufferCallbackFunction_ = nullptr;

    std::function<void(int, int, int, int)> keyCallbackFunction_ = nullptr;
    std::function<void(unsigned int)> charCallbackFunction_ = nullptr;
    std::function<void(int, int, int)> mouseButtonCallbackFunction_ = nullptr;
    std::function<void(double, double)> cursorPositionCallbackFunction_ = nullptr;
    std::function<void(int)> cursorEnterCallbackFunction_ = nullptr;
    std::function<void(double, double)> scrollCallbackFunction_ = nullptr;
    std::function<void(int, const char**)> dropCallbackFunction_ = nullptr;
  };
}
